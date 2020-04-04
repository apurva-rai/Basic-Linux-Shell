/*@Meet Kapadia
@Node.h
@9/26/2018
@Brief: This file lays out the functions we will define for node class
as well as makes a private templated node pointer*/

#ifndef NODE_H
#define NODE_H

#include <iostream>


class Node
{

public:

	Node(int value);

	void setValue(int value);

	int getValue()const;

	void setNext(Node* prev);

 	Node* getNext()const;

private:
	int m_value;
	Node* m_next;

};
#endif
