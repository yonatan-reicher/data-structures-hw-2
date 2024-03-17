#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "tree.h"
#include <cmath>

unsigned int hash(unsigned int x) {
    constexpr unsigned int MULTIPLIER = 0x45d9f3b;
    x = ((x >> 16) ^ x) * MULTIPLIER;
    x = ((x >> 16) ^ x) * MULTIPLIER;
    x = (x >> 16) ^ x;
    return x;
}

template <typename K, typename V>
class HashTable {
    // Load factor = size / capacity
    constexpr static double MIN_LOAD_FACTOR = 0.25;
    constexpr static double MAX_LOAD_FACTOR = 0.75;
    constexpr static double WANTED_LOAD_FACTOR = 0.5;
    constexpr static unsigned int START_CAPACITY = 8;

    std::unique_ptr<Tree<K, V>[]> m_array;
    int m_size;
    int m_capacity;
    
public:
    HashTable()
        : m_array(new Tree<K, V>[START_CAPACITY]),
        m_size(0),
        m_capacity(START_CAPACITY) {}

    /* This function "fixes" the hash table.
     * 1. It reallocates the array according to the load factor.
     * 2. It reinserts all the elements.
     *
     * Complexity: O(n)
     */
    void rehash() {
        int newCapacity = std::ceil(m_size / WANTED_LOAD_FACTOR);

        std::unique_ptr<Tree<K, V>[]> newArray(new Tree<K, V>[newCapacity]);
        for (int i = 0; i < m_capacity; i++) {
            Tree<K, V>& tree = m_array[i];
            tree.toArray();
        }
    }

    bool contains(int key) const;
    V& insert(int key, V value) {
        throw std::runtime_error("Not implemented");
    }
    const V& find(int key) const;
    V& find(int key);
    V remove(int key);
};

#endif
