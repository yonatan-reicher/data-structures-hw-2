#include "../hashtable.h"
#include <iostream>

int main() {
    HashTable<int, double> table;
    table.insert(1, 3.14);
    table.insert(2, 2.71);
    return 0;
}
