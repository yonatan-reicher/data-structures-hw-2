#include "../hashtable.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <set>

std::vector<int> acsendingData = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
std::vector<int> randomData = std::vector<int>{ 23, 1, 2, 5, 6, 7, 3, 14, 9, 10 };

int countDistinct(std::vector<int> data) {
    std::set<int> distinct;
    for (size_t i = 0; i < data.size(); i++) {
        distinct.insert(data[i]);
    }
    return distinct.size();
}

void testData(const std::vector<int>& data) {
    int distinct = countDistinct(data);
    HashTable<int, int> table;

    for (size_t i = 0; i < data.size(); i++) {
        table.insert(data[i], data[i]);
    }
    assert(table.size() == distinct);

    for (size_t i = 0; i < data.size(); i++) {
        assert(table.get(data[i]) == data[i]);
    }
    for (size_t i = 0; i < data.size(); i++) {
        if (table.contains(data[i])) {
            assert(table.remove(data[i]) == data[i]);
        }
    }
    assert(table.size() == 0);
}

int main() {

    testData(acsendingData);
    testData(randomData);

    std::cout << "HashTable tests passed" << std::endl;

    return 0;
}
