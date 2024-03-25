
CC = g++
CFLAGS = -std=c++11 -g -Wall
# CFLAGS = -std=c++11 -DNDEBUG -Wall *.cpp

SOURCES_NO_MAIN = $(filter-out main24a2.cpp,$(wildcard *.cpp))

test:
	$(CC) $(CFLAGS) -o test Tests/test1.cpp $(SOURCES_NO_MAIN)
	./test

test-hashtable:
	$(CC) $(CFLAGS) -o test-hashtable Tests/test-hashtable.cpp
	./test-hashtable

test-tree:
	$(CC) $(CFLAGS) -o test-tree Tests/testTree.cpp
	./test-tree
