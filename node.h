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

// This mostly exists to allow Team to specialize it.
template <class T>
int getPower(const T& data) {
    return 0;
}

template <class K, class T>
class Node {
    std::unique_ptr<Node> m_left;
    std::unique_ptr<Node> m_right;
    int m_height;
    // The number of nodes in the subtree including this node.
    int m_count;
    // This field is the `Ido Galil` field for our purposes.
    int m_addWins;
    int m_maxRank;
public:
    K key;
    T data;

    Node(K key, T data)
        : m_left(nullptr), m_right(nullptr)
        , m_height(0)
        , m_addWins(0)
        , m_maxRank(0)
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

    int addWins() const {
        return m_addWins;
    }

    void setAddWins(int newAddWins) {
        m_addWins = newAddWins;
        updateMaxRank();
    }

    int maxRank() const {
        return m_maxRank;
    }

private:
    void updateHeightAndCount() {
        m_height = 1 + std::max(height(m_left), height(m_right));
        m_count = 1 + count(m_left) + count(m_right);
    }

    void onChildIn(Node* child) {
        if (!child) return;
        child->m_addWins -= m_addWins;
        updateMaxRank();
    }

    void onChildOut(Node* child) {
        if (!child) return;
        child->m_addWins += m_addWins;
        updateMaxRank();
    }

    void updateMaxRank() {
        m_maxRank = m_addWins + std::max(
            std::max(
                m_left ? m_left->maxRank() : 0,
                m_right ? m_right->maxRank() : 0
            ),
            getPower(data)
        );
    }


    template <class L, class U>
    friend int height(const std::unique_ptr<Node<L, U>>&);
    template <class L, class U>
    friend int balanceFactor(const std::unique_ptr<Node<L, U>>&);
    template <class L, class U>
    friend int count(const std::unique_ptr<Node<L, U>>&);
};

#endif
