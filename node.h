#ifndef DSNODE_H
#define DSNODE_H

#include <memory>

template <class K, class T>
class Node;

// These are functions that cannot be methods because they can be called on a nullptr

template <class K, class T>
int height(const std::unique_ptr<Node<K, T>>& root) {
    if (root == nullptr) {
        return -1;
    }
    return root->m_height;
}

template <class K, class T>
int balanceFactor(const std::unique_ptr<Node<K, T>>& root) {
    if (root == nullptr) {
        return 0;
    }
    return height(root->m_left) - height(root->m_right);
}

template <class K, class T>
int count(const std::unique_ptr<Node<K, T>>& root) {
    if (root == nullptr) {
        return 0;
    }
    return root->m_count;
}

template <class K, class T>
class Node {
    std::unique_ptr<Node> m_left;
    std::unique_ptr<Node> m_right;
    int m_height;
    // The number of nodes in the subtree including this node.
    int m_count;
    // This field is the `Ido Galil` field for our purposes. This will probably
    // only be used for the sum of wins a team has gained in this homework
    // project.
    int m_partialSum;
    // TODO: Add a maxRank field.
public:
    K key;
    T data;

    Node(K key, T data)
        : m_left(nullptr), m_right(nullptr)
        , m_height(0)
        , m_partialSum(0)
        , key(std::move(key))
        , data(std::move(data))
    {
        updateHeightAndCount();
    }

    std::unique_ptr<Node> setLeft(std::unique_ptr<Node> newLeft) {
        onChildIn(newLeft.get());
        std::unique_ptr<Node> ret = std::move(m_left);
        m_left = std::move(newLeft);
        updateHeightAndCount();
        onChildOut(ret.get());
        return ret;
    }

    std::unique_ptr<Node> setRight(std::unique_ptr<Node> newRight) {
        onChildIn(newRight.get());
        std::unique_ptr<Node> ret = std::move(m_right);
        m_right = std::move(newRight);
        updateHeightAndCount();
        onChildOut(ret.get());
        return ret;
    }

    std::unique_ptr<Node> popLeft() {
        return setLeft(nullptr);
    }

    std::unique_ptr<Node> popRight() {
        return setRight(nullptr);
    }

    const std::unique_ptr<Node>& getLeft() const {
        return m_left;
    }

    const std::unique_ptr<Node>& getRight() const {
        return m_right;
    }

    int partialSum() const {
        return m_partialSum;
    }

    int& partialSum() {
        return m_partialSum;
    }

private:
    void updateHeightAndCount() {
        m_height = 1 + std::max(height(m_left), height(m_right));
        m_count = 1 + count(m_left) + count(m_right);
    }

    void onChildIn(Node* child) const {
        if (!child) return;
        child->m_partialSum -= m_partialSum;
    }

    void onChildOut(Node* child) const {
        if (!child) return;
        child->m_partialSum += m_partialSum;
    }

    template <class L, class U>
    friend int height(const std::unique_ptr<Node<L, U>>&);
    template <class L, class U>
    friend int balanceFactor(const std::unique_ptr<Node<L, U>>&);
    template <class L, class U>
    friend int count(const std::unique_ptr<Node<L, U>>&);
};

#endif
