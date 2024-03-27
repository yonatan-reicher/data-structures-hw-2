#ifndef DSTREE_H
#define DSTREE_H

#include <memory>
#include <cassert>
#include <iostream>
#include "node.h"

template <class K>
struct NotFoundException : public std::exception {
    K key;

    explicit NotFoundException(K key) : key(std::move(key)) {}

    const char* what() const throw() {
        return "Key not found";
    }
};

template <class K, class T>
auto operator<<(std::ostream& os, const std::unique_ptr<Node<K, T>>& node) -> std::ostream&;

// Functions marked with "bt" are binary tree functions.
// Functions marked with "avl" are AVL tree functions.

template <class K, class T>
T& btInsert(std::unique_ptr<Node<K, T>>& root, const K& key, T data, bool* alreadyExisted) {
    if (root == nullptr) {
        root = std::unique_ptr<Node<K, T>>(new Node<K, T>(key, std::move(data)));
        return root->data;
    }

    if (key == root->key) {
        root->data = std::move(data);
        if (alreadyExisted != nullptr) {
            *alreadyExisted = true;
        }
        return root->data;
    }

    if (key < root->key) {
        std::unique_ptr<Node<K, T>> left = root->popLeft();
        T& ret = btInsert(left, key, std::move(data), alreadyExisted);
        root->setLeft(std::move(left));
        return ret;
    } else {
        std::unique_ptr<Node<K, T>> right = root->popRight();
        T& ret = btInsert(right, key, std::move(data), alreadyExisted);
        root->setRight(std::move(right));
        return ret;
    }
}

template <class K, class T>
std::unique_ptr<Node<K, T>> popRightmost(std::unique_ptr<Node<K, T>>& node) {
    assert(node != nullptr);

    if (node->getRight() == nullptr) {
        std::unique_ptr<Node<K, T>> ret = std::move(node);
        node = ret->popLeft();
        return ret;
    }

    std::unique_ptr<Node<K, T>> right = node->popRight();
    std::unique_ptr<Node<K, T>> ret = popRightmost(right);
    node->setRight(std::move(right));
    return ret;
}

template <class K, class T>
T btRemove(std::unique_ptr<Node<K, T>>& root, const K& key, K* keyOfSwitched) {
    if (root == nullptr) {
        throw NotFoundException<K>(key);
    }

    if (root->key == key && root->getLeft() == nullptr) {
        T result = std::move(root->data);
        root = root->popRight();
        return result;
    } else if (root->key == key && root->getRight() == nullptr) {
        T result = std::move(root->data);
        root = root->popLeft();
        return result;
    } else if (root->key == key) {
        /*     5              5            5   4
         *    / \                             / \
         *    2  7    ==>    2  7    ==>     2   7
         *   / \            /|              / \
         *  1   4          1 | 4           1   3
         *     /             |      
         *    3              3      
         */
        // Find the rightmost node in the left subtree. Swap the data.
        // This should be done *carefully* so no uniqueptr is cleaned up!
        T result = std::move(root->data);
        std::unique_ptr<Node<K, T>> left = root->popLeft();
        std::unique_ptr<Node<K, T>> right = root->popRight();
        std::unique_ptr<Node<K, T>> rightmost = popRightmost(left);
        root = std::move(rightmost);
        root->setLeft(std::move(left));
        root->setRight(std::move(right));
        if (keyOfSwitched != nullptr) {
            *keyOfSwitched = root->key;
        }
        return result;
    } else if (key < root->key) {
        std::unique_ptr<Node<K, T>> left = root->popLeft();
        T result = btRemove(left, key, keyOfSwitched);
        root->setLeft(std::move(left));
        return result;
    } else {
        std::unique_ptr<Node<K, T>> right = root->popRight();
        T result = btRemove(right, key, keyOfSwitched);
        root->setRight(std::move(right));
        return result;
    }
}

template <class K, class T>
T& btGet(Node<K, T>* root, const K& key) {
    if (root == nullptr) {
        throw NotFoundException<K>(key);
    }

    if (root->key == key) {
        return root->data;
    }

    if (key < root->key) {
        return btGet(root->getLeft().get(), key);
    } else {
        return btGet(root->getRight().get(), key);
    }
}

template <class K, class T>
void rotate(std::unique_ptr<Node<K, T>>& root, bool right) {
    assert(root != nullptr);
    const std::unique_ptr<Node<K, T>>& bRef = right ? root->getLeft() : root->getRight();
    assert(bRef != nullptr);

    // This is the action we want to perform:
    /*        A    right   B
     *       / \    ==>   / \
     *      B   C   <==  D   A
     *     / \     left     / \
     *    D   E            E   C
     *
     *    (Here C, D, and E can all be nulls)
     */

    // Here we use the names from the diagram above (of the right rotation).
    std::unique_ptr<Node<K, T>> a = std::move(root);
    std::unique_ptr<Node<K, T>> b = right ? a->popLeft() : a->popRight();
    std::unique_ptr<Node<K, T>> e = right ? b->popRight() : b->popLeft();
    // C and D are not needed.

    // Now we reassemble the tree. Must go from bottom to top because of moves.
    if (right) {
        a->setLeft(std::move(e));
        b->setRight(std::move(a));
        root = std::move(b);
    } else {
        a->setRight(std::move(e));
        b->setLeft(std::move(a));
        root = std::move(b);
    }
}

template <class K, class T>
void rotateLLOrRR(std::unique_ptr<Node<K, T>>& root, bool ll) {
    // This is just a single rotation.
    rotate(root, ll);
}

template <class K, class T>
void rotateLROrRL(std::unique_ptr<Node<K, T>>& root, bool lr) {
    /* This is a double rotation.
     *      A             E
     *     / \    LR     / \
     *    B   C   ==>   B   A
     *   / \       ^   / \ / \
     *  D   E      |  D  F G  C
     *     / \     |   
     *    F   G    --- This is a left rotation on B then right rotation on A.
     */

    std::unique_ptr<Node<K, T>> child = lr ? root->popLeft() : root->popRight();
    rotate(child, !lr);
    if (lr) root->setLeft(std::move(child));
    else root->setRight(std::move(child));

    rotate(root, lr);
}

enum class RebalanceType {
    LL, RR, LR, RL, NONE
};

// Why is this a template? Because if it wasn't it would be redefined multiple
// times and that won't compile.
template <class K, class T> 
RebalanceType chooseRebalanceByBalanceFactor(int atRoot, int atLeft, int atRight) {
    assert(-2 <= atRoot && atRoot <= 2);
    assert(-1 <= atLeft && atLeft <= 1);
    assert(-1 <= atRight && atRight <= 1);

    if (atRoot == 2 && atLeft > -1) return RebalanceType::LL;
    if (atRoot == 2 && atLeft == -1) return RebalanceType::LR;
    if (atRoot == -2 && atRight < 1) return RebalanceType::RR;
    if (atRoot == -2 && atRight == 1) return RebalanceType::RL;
    return RebalanceType::NONE;
}

// Returns true iff the tree was rebalanced.
template <class K, class T>
bool rebalanceRoot(std::unique_ptr<Node<K, T>>& root) {
    if (root == nullptr) {
        return false;
    }

    int atRoot = balanceFactor(root);
    int atLeft = balanceFactor(root->getLeft());
    int atRight = balanceFactor(root->getRight());

    switch (chooseRebalanceByBalanceFactor<K, T>(atRoot, atLeft, atRight)) {
        case RebalanceType::LL:
            rotateLLOrRR(root, true);
            return true;
        case RebalanceType::RR:
            rotateLLOrRR(root, false);
            return true;
        case RebalanceType::LR:
            rotateLROrRL(root, true);
            return true;
        case RebalanceType::RL:
            rotateLROrRL(root, false);
            return true;
        case RebalanceType::NONE:
            return false;
        default:
            throw std::exception();
    }
}

template <class K, class T>
bool rebalanceBranch(std::unique_ptr<Node<K, T>>& root, const K& pathKey, bool once) {
    if (root == nullptr) {
        return false;
    }

    bool rightChild = pathKey > root->key;
    std::unique_ptr<Node<K, T>> next = rightChild ? root->popRight() : root->popLeft();

    bool rebalanced = rebalanceBranch(next, pathKey, once);

    if (rightChild) root->setRight(std::move(next));
    else root->setLeft(std::move(next));

    if (!rebalanced || !once) {
        rebalanced = rebalanceRoot(root);
    }

    return rebalanced;
}

template <class K, class T>
const K& getRightmostKey(const std::unique_ptr<Node<K, T>>& root) {
    assert(root != nullptr);
    if (root->getRight() == nullptr) {
        return root->key;
    }
    return getRightmostKey(root->getRight());
}

template <class K, class T>
const K& getAvlRemoveRebalancePath(const std::unique_ptr<Node<K, T>>& root, const K& key) {
    if (root == nullptr) return key;

    if (key == root->key) {
        if (root->getLeft() != nullptr) {
            return getRightmostKey(root->getLeft());
        } else {
            return root->key;
        }
    }
    if (key < root->key) {
        return getAvlRemoveRebalancePath(root->getLeft(), key);
    } else {
        return getAvlRemoveRebalancePath(root->getRight(), key);
    }
}

template <class K, class T>
T& avlInsert(std::unique_ptr<Node<K, T>>& root, const K& key, T data, bool* wasInserted) {
    T& dataReference = btInsert(root, key, std::move(data), wasInserted);

    rebalanceBranch(root, key, true);

    return dataReference;
}

template <class K, class T>
T avlRemove(std::unique_ptr<Node<K, T>>& root, const K& key) {
    K pathKey = key;
    T data = btRemove(root, key, &pathKey);
    pathKey = getAvlRemoveRebalancePath(root, pathKey);
    rebalanceBranch(root, pathKey, false);

    return data;
}

template <class K, class T>
const std::unique_ptr<Node<K, T>>& getMaximum(const std::unique_ptr<Node<K, T>>& root) {
    if (root == nullptr) {
        return root;
    }
    if (root->getRight() == nullptr) {
        return root;
    }
    return getMaximum(root->getRight());
}

template <class K, class T>
const std::unique_ptr<Node<K, T>>& getMinimum(const std::unique_ptr<Node<K, T>>& root) {
    if (root == nullptr) {
        return root;
    }
    if (root->getLeft() == nullptr) {
        return root;
    }
    return getMinimum(root->getLeft());
}

// Returns the number of elements written
template <class K, class T>
int treeToArray(const std::unique_ptr<Node<K, T>>& root, T** array) {
    if (root == nullptr) {
        return 0;
    }

    int written = 0;
    written += treeToArray(root->getLeft(), array);
    array[written] = &root->data;
    written++;
    written += treeToArray(root->getRight(), array + written);
    return written;
}

// Returns the number of elements written
template <class K, class T>
int treeKeysToArray(const std::unique_ptr<Node<K, T>>& root, K** array) {
    if (root == nullptr) {
        return 0;
    }

    int written = 0;
    written += treeKeysToArray(root->getLeft(), array);
    array[written] = &root->key;
    written++;
    written += treeKeysToArray(root->getRight(), array + written);
    return written;
}

template <class K, class T, class F>
std::unique_ptr<Node<K, T>> treeFromArray(T* array, int size, F& keyGenerator) {
    if (size == 0) {
        return nullptr;
    }

    if (size % 2 == 1) {
        int leftSize = size / 2;
        int rightSize = size / 2;
        K key = keyGenerator(array[leftSize]);
        std::unique_ptr<Node<K, T>> root = std::unique_ptr<Node<K, T>>(
            new Node<K, T>(std::move(key), std::move(array[leftSize]))
        );
        root->setLeft(treeFromArray<K, T, F>(array, leftSize, keyGenerator));
        root->setRight(treeFromArray<K, T, F>(array + leftSize + 1, rightSize, keyGenerator));
        return root;
    }
    else {
        assert(size % 2 == 0);
        int leftSize = size / 2 - 1;
        int rightSize = size / 2;
        K key = keyGenerator(array[leftSize]);
        std::unique_ptr<Node<K, T>> root = std::unique_ptr<Node<K, T>>(
            new Node<K, T>(std::move(key), std::move(array[leftSize]))
        );
        root->setLeft(treeFromArray<K, T, F>(array, leftSize, keyGenerator));
        root->setRight(treeFromArray<K, T, F>(array + leftSize + 1, rightSize, keyGenerator));
        return root;
    }
}

template <class K, class T>
const std::unique_ptr<Node<K, T>>& getByIndex(const std::unique_ptr<Node<K, T>>& root, int index) {
    if (root == nullptr) {
        throw std::runtime_error("Index out of bounds");
    }

    int leftSize = count(root->getLeft());
    if (index == leftSize) {
        return root;
    }
    if (index < leftSize) {
        return getByIndex(root->getLeft(), index);
    }
    return getByIndex(root->getRight(), index - leftSize - 1);
}

// Gets and sets the branch sum (= sum of addWins) of a given node.
template <class K, class T>
int branchSum(Node<K, T>* root, const K& key, int value) {
    if (root == nullptr) return 0;

    if (root->key == key) {
        int previous = root->addWins();
        root->setAddWins(value);
        return previous;
    }

    if (root->key < key) {
        std::unique_ptr<Node<K, T>> right = root->popRight();
        int result = branchSum(right.get(), key, value);
        root->setRight(std::move(right));
        return result;
    } else {
        std::unique_ptr<Node<K, T>> left = root->popLeft();
        int result = branchSum(left.get(), key, value);
        root->setLeft(std::move(left));
        return result;
    }
}

template <class K, class T>
void addBranchSumUpTo(Node<K, T>* root, int i, int add) {
    if (root == nullptr || i < 0) return;

    int rootIndex = count(root->getLeft());
    if (rootIndex <= i) {
        root->setAddWins(root->addWins() + add);
        // Offset the right branch so we only increase to the left.
        if (root->getRight()) {
            root->getRight()->setAddWins(root->addWins() - add);
        }

        addBranchSumUpTo(root->getRight().get(), i - rootIndex - 1, add);
    } else {
        addBranchSumUpTo(root->getLeft().get(), i, add);
    }
}

template <class K, class T>
class Tree {
private:
    using TreeNode = Node<K, T>;

    std::unique_ptr<TreeNode> root;
    int m_size;
    TreeNode* m_minimum;
    TreeNode* m_maximum;
    TreeNode* m_middle;

    void updateMinAndMaxAndMiddle() {
        std::unique_ptr<TreeNode>& min = const_cast<std::unique_ptr<TreeNode>&>(getMinimum(root));
        std::unique_ptr<TreeNode>& max = const_cast<std::unique_ptr<TreeNode>&>(getMaximum(root));
        m_minimum = min == nullptr ? nullptr : min.get();
        m_maximum = max == nullptr ? nullptr : max.get();
        assert(m_size == 0 ? m_minimum == nullptr : m_minimum != nullptr);
        assert(m_size == 0 ? m_maximum == nullptr : m_maximum != nullptr);
        assert(m_size == 0 || !(m_minimum->key > m_maximum->key));

        int middleIndex = m_size / 2;
        m_middle = (
            m_size == 0 ? nullptr :
            const_cast<std::unique_ptr<TreeNode>&>(getByIndex(root, middleIndex)).get()
        );
        assert(m_size == 0 ? m_middle == nullptr : m_middle != nullptr);
    }
public:
    Tree() {
        root = nullptr;
        m_size = 0;
        updateMinAndMaxAndMiddle();
    }
    template <class F>
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;
    Tree(Tree&&) = default;
    Tree& operator=(Tree&&) = default;

    T& insert(const K& key, T data) {
        bool wasAlreadyInserted = false;
        T& dataReference = avlInsert(root, key, std::move(data), &wasAlreadyInserted);
        if (!wasAlreadyInserted) {
            m_size++; // Must happen after the insert so we don't count failed calls.
        }
        // Update minimum and maximum. O(log n) time so it's fine.
        updateMinAndMaxAndMiddle();
        return dataReference;
    }

    T remove(const K& key) {
        T data = avlRemove(root, key);
        m_size--; // Must happen after the remove so we don't count failed calls.
        // Update minimum and maximum. O(log n) time so it's fine.
        updateMinAndMaxAndMiddle();
        return data;
    }

    T& get(const K& key) {
        return btGet(root.get(), key);
    }

    const T& get(const K& key) const {
        return btGet(root.get(), key);
    }


    // Branch sums start at 0. Branch sum is a property that can be added to on
    // any number of nodes in log(n) time. Used for addWins in the original
    // problem!
    // Note: Must be called on a key that is in the tree.
    int branchSum(const K& key) const {
        int result = branchSum(root.get(), key, 0);
        branchSum(root.get(), key, result);
        return result;
    }

    void addWinsInRange(int lowIndex, int highIndex, int add) {
        assert(lowIndex <= highIndex);
        assert(highIndex < m_size);
        assert(lowIndex >= 0);
        if (lowIndex > 0) {
            addWinsInRange(0, highIndex, add);
            addWinsInRange(0, lowIndex - 1, -add);
            return;
        }

        assert(lowIndex == 0);
        addBranchSumUpTo(root.get(), highIndex, add);
    }

    int getMaxRank() const {
        return m_size == 0 ? 0 : root->maxRank();
    }

    bool contains(const K& key) const {
        try {
            get(key);
            return true;
        } catch (const NotFoundException<K>& e) {
            return false;
        }
    }

    int size() const {
        return m_size;
    }

    const T* minimum() const {
        return m_minimum->data;
    }

    T* minimum() {
        return m_minimum->data;
    }

    const K& minimumKey() const {
        return m_minimum->key;
    }

    const T* maximum() const {
        return m_maximum->data;
    }

    T* maximum() {
        return m_maximum->data;
    }

    const K& maximumKey() const {
        return m_maximum->key;
    }

    const K& middleKey() const {
        return m_middle->key;
    }

    // Return value must be deleted by the caller using delete[].
    // The pointers point to the data in the tree, they are not copies.
    T** toArray() const {
        T** array = new T*[m_size];
        int written = treeToArray(root, array);
        assert(written == m_size);
        return array;
    }

    K** keysToArray() const {
        K** array = new K*[m_size];
        int written = treeKeysToArray(root, array);
        assert(written == m_size);
        return array;
    }

    // Input array must be deleted by the caller using delete[].
    // Input array MUST be sorted and have NO duplicates.
    // The elements in the array are MOVED into the tree.
    // TODO: Maybe this should just be a constructor...
    template <class F>
    static Tree fromArray(T* array, int size, F& keyGenerator) {
        Tree ret;
        ret.root = treeFromArray<K, T, F>(array, size, keyGenerator);
        ret.m_size = size;
        ret.updateMinAndMaxAndMiddle();
        return ret;
    }

    friend auto operator<<(std::ostream& os, const Tree& tree) -> std::ostream& { 
        return os << tree.root;
    }
};

template <class K, class T>
auto operator<<(std::ostream& os, const std::unique_ptr<Node<K, T>>& node) -> std::ostream& { 
    os << '[';
    
    if (node) {
        if (node->getLeft() != nullptr) {
            os << node->getLeft();
        }

        os << node->key << ":" << node->data;

        if (node->getRight() != nullptr) {
            os << node->getRight();
        }
    }

    return os << ']';
}

#endif
