#ifndef DSNODE_H
#define DSNODE_H

#include <memory>

template <class K, class T>
class Node;

// These are functions that cannot be methods because they can be called on a nullptr

template <class K, class T>
int height(const Node<K, T>* root) {
    if (root == nullptr) {
        return -1;
    }
    return root->m_height;
}

template <class K, class T>
int balanceFactor(const Node<K, T>* root) {
    if (root == nullptr) {
        return 0;
    }
    return height(root->m_left.get()) - height(root->m_right.get());
}

template <class K, class T>
int count(const Node<K, T>* root) {
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
        onChildOut(ret.get());
        updateHeightAndCount();
        return ret;
    }

    std::unique_ptr<Node> setRight(std::unique_ptr<Node> newRight) {
        onChildIn(newRight.get());
        std::unique_ptr<Node> ret = std::move(m_right);
        m_right = std::move(newRight);
        onChildOut(ret.get());
        updateHeightAndCount();
        return ret;
    }

    std::unique_ptr<Node> popLeft() {
        return setLeft(nullptr);
    }

    std::unique_ptr<Node> popRight() {
        return setRight(nullptr);
    }

    // TODO: Make const.
    const Node* getLeft() const {
        return m_left.get();
    }

    // TODO: Make const.
    const Node* getRight() const {
        return m_right.get();
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
        m_height = 1 + std::max(height(m_left.get()), height(m_right.get()));
        m_count = 1 + count(m_left.get()) + count(m_right.get());
        updateMaxRank();
    }

    void onChildIn(Node* child) {
        if (!child) return;
        child->m_addWins -= m_addWins;
        child->updateMaxRank();
    }

    void onChildOut(Node* child) {
        if (!child) return;
        child->m_addWins += m_addWins;
        child->updateMaxRank();
    }

    void updateMaxRank() {
        m_maxRank = m_addWins + std::max(
            std::max(
                m_left ? m_left->maxRank() : 0,
                m_right ? m_right->maxRank() : 0
            ),
            getPower<T>(data)
        );
    }


    template <class L, class U>
    friend int height(const Node<L, U>*);
    template <class L, class U>
    friend int balanceFactor(const Node<L, U>*);
    template <class L, class U>
    friend int count(const Node<L, U>*);
};

template class Node<int, int*>;

#endif
