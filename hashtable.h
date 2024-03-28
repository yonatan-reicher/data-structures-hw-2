#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "tree.h"
#include <cmath>


/*
 *  O(1) armortized time! Yay
 *
 *  Notes:
 *  1. Elements *don't* stay in same place in memory!
 *     This means that you SHOULD NOT keep references returned by get.
 *  2. This is generic on keys but we can only hash thingts that convert to
 *     unsigned int. 
 *  3. Does not support duplicate keys!
 */
template <typename K, typename V>
class HashTable {
    // Load factor = size / amount of cells
    constexpr static double MIN_LOAD_FACTOR = 0.25;
    constexpr static double MAX_LOAD_FACTOR = 0.75;
    constexpr static double WANTED_LOAD_FACTOR = 0.5;
    constexpr static unsigned int START_CELLS = 8;

    std::unique_ptr<Tree<K, V>[]> m_array;
    // Amount of elements in the hash table
    int m_size;
    // Amount of cells (trees) in the hash table
    int m_cellsCount;
    
public:
    HashTable()
        : m_array(new Tree<K, V>[START_CELLS]),
        m_size(0),
        m_cellsCount(START_CELLS) {}

    /* This function "fixes" the hash table.
     * 1. It reallocates the array according to the load factor.
     * 2. It reinserts all the elements.
     *
     * Complexity: O(n)
     */
    void rehash() {
        int newCellCount = std::ceil(m_size / WANTED_LOAD_FACTOR);

        std::unique_ptr<Tree<K, V>[]> newArray(new Tree<K, V>[newCellCount]);
        for (int i = 0; i < m_cellsCount; i++) {
            Tree<K, V>& tree = m_array[i];

            // Iterate through the tree element by element.
            std::unique_ptr<K[]> keys;
            std::unique_ptr<V[]> values;
            tree.toArrays(keys, values);
            for (int j = 0; j < tree.size(); j++) {
                unsigned int index = hash(keys[j]) % newCellCount;
                newArray[index].insert(std::move(keys[j]), std::move(values[j]));
            }
        }

        // Now switch the arrays!
        m_array = std::move(newArray);
        m_cellsCount = newCellCount;
    }

    void afterChange() {
        if ((double)m_size / m_cellsCount > MAX_LOAD_FACTOR) {
            rehash();
        }

        // Don't resize down if we are at the minimum size!
        if (m_size < (int)START_CELLS) return;

        if ((double)m_size / m_cellsCount < MIN_LOAD_FACTOR) {
            rehash();
        }
    }

    bool contains(const K& key) const {
        unsigned int index = hash(key) % m_cellsCount;
        return m_array[index].contains(key);
    }

    void insert(K key, V value) {
        unsigned int index = hash(key) % m_cellsCount;
        m_array[index].insert(key, std::move(value));
        m_size++;

        afterChange();

        // In contrast to tree, we don't return a reference to the value because
        // afterChange might change it's location in memory so I thought it's
        // better to just not call get for no reason to find it.
    }

    // Do not keep the reference returned by get! See the class comment.
    const V& get(K key) const {
        unsigned int index = hash(key) % m_cellsCount;
        return m_array[index].get(key);
    }
    // Do not keep the reference returned by get! See the class comment.
    V& get(K key) {
        unsigned int index = hash(key) % m_cellsCount;
        return m_array[index].get(key);
    }

    V remove(K key) {
        unsigned int index = hash(key) % m_cellsCount;
        V value = m_array[index].remove(key);
        m_size--;

        afterChange();

        return value;
    }

    int size() const {
        return m_size;
    }
private:
    static unsigned int hash(unsigned int x) {
        // According to the all knowing Piazza, this constitutes a good hash function.
        // I'll keep this here just to explicitly cast to unsigned int.
        return x;
    }
};

template class HashTable<unsigned int, unsigned int>;

#endif
