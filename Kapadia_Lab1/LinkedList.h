


#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include "Node.h"

using namespace std;

class LinkedList
{
public:

	LinkedList();

	~LinkedList();

	bool isEmpty() const;

	int getLength() const;

	void insert(int position);

	void remove(int position);

	void deleteFunc(int position);

	void removeDup(int value);

	bool Find(int value);

	void FindNext(int value);

	void print();

	void printReverse();

	void printAt(int position);

	int getVal(int val);


private:
	int itemCount;
	Node* headPtr;
};
#endif
