
CC = g++
CFLAGS = -std=c++11 -g -Wall
# CFLAGS = -std=c++11 -DNDEBUG -Wall *.cpp

test-hashtable:
	$(CC) $(CFLAGS) -o test-hashtable Tests/test-hashtable.cpp
	./test-hashtable

