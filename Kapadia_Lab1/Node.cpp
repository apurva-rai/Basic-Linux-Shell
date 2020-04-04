/*@Meet Kapadia
@Node.cpp
@9/26/2018
@Brief: implements all the node.h functions*/

#include <iostream>
#include "Node.h"



Node::Node(int value)
{
  m_value=value;
  m_next = nullptr;

}


void Node::setValue(int value)
{
  m_value = value;

}


int Node::getValue()const
{
  return m_value;

}


void Node::setNext(Node* prev)
{
  m_next = prev;

}


Node* Node::getNext()const
{
  return m_next;

}
