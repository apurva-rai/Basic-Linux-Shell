#include "LinkedList.h"
using namespace std;



LinkedList::LinkedList()
{
  headPtr = nullptr;
  itemCount = 0;
}



LinkedList::~LinkedList()
{
  while(itemCount != 0)
  {
    Node* node = headPtr->getNext();
    delete headPtr;
    headPtr = node;
    itemCount--;
  }
}



bool LinkedList::isEmpty() const
{
  if(headPtr == nullptr)
  {
    return(true);
  }
  else
  {
    return(false);
  }
}

int LinkedList::getLength() const
{
  return(itemCount);
}



void LinkedList::insert(int position)
{
  Node* node = new Node(position);
  node->setNext(headPtr);
  headPtr= node;
  itemCount++;
}



void LinkedList::remove(int position)
{
  Node* node = headPtr;
  Node* node2 = headPtr;
  Node* temp = nullptr;
  if(itemCount == 0)
  {
    cout<<"List is empty"<<endl;
  }
  else if (position == 1)
  {
    temp = headPtr;
    headPtr = headPtr->getNext();
    delete temp;
  }
  else if (position == itemCount)
  {
    temp = headPtr;
    for(int i =1; i <= itemCount; i++)
    {
      node = node->getNext();
    }
  }
  else
  {
    for(int i = 1;  i <= itemCount;i++)
    {
      node = node->getNext();
    }
    temp = headPtr;
    for (int i =1;i<position-1;i++)
    {
      temp=temp->getNext();
    }
    for(int i = 1; i<position+1;i++)
    {
      node2 = node2->getNext();
    }
    temp->setNext(node2);
    delete node;
  }
  itemCount--;
}

void LinkedList::deleteFunc(int position)
{
  Node* node = headPtr;
  Node* node2 = nullptr;
  if(isEmpty())
  {
    cout<<"list is empty"<<endl;
  }
  if(Find(position))
  {
    while (node != nullptr)
    {
      if(node->getValue() == position)
      {
        node2 = node;
      }
      node=node->getNext();
    }
    if(node2==headPtr)
    {
      headPtr = node2->getNext();
      delete node2;
      itemCount--;
      	cout << "The last occurrence of " << position << " has been deleted from the list";
    }
    else if(node2!=nullptr && node2->getNext()==nullptr)
    {
      node = headPtr;
      while(node->getNext() != node2)
      {
          node = node->getNext();
      }
      node->setNext(nullptr);
      delete node2;
      itemCount--;
      	cout << "The last occurrence of " << position << " has been deleted from the list";
    }
    else if(node2!=nullptr && node2->getNext()!=nullptr)
    {
      Node* temp = headPtr;
      while(temp->getNext() != node2)
      {
        temp = temp->getNext();
      }
      temp->setNext(node2->getNext());
      delete node2;
      itemCount--;
	    cout << "The last occurrence of " << position << " has been deleted from the list";    }
  }
  else
  {
    cout<<position<<" was not in the list"<<endl;
  }
}

void LinkedList::removeDup(int value)
{
  bool noDup=false;
  for(int i=1;i<=itemCount;i++)
  {
      if(getVal(i)==value)
      {
        if(noDup)
        {
          remove(i);
        }
        else
        {
          noDup=true;
        }
      }
  }
}



bool LinkedList::Find(int value)
{
  Node* node = headPtr;
  for(int i = 1; i <= itemCount; i++)
  {
      if(node->getValue() == value)
      return true;
      node = node->getNext();
  }
  return false;
}



void LinkedList::FindNext(int value)
{
  Node* node = headPtr;
  if(Find(value))
  {
    while (node != nullptr && node->getNext()!=nullptr)
    {
      if(node->getValue()==value)
      {
        cout<<node->getNext()->getValue()<<" is next after "<<value<<endl;
        return;
      }
      node = node->getNext();
    }
  }
  else
  {
    cout<<"Not in list"<<endl;
  }
}



void LinkedList::print()
{
  Node* node = headPtr;
  while (node != nullptr)
  {
    std::cout<<node->getValue()<<" ";
    node = node->getNext();
  }
  cout<<endl;
}



void LinkedList::printAt(int position)
{
  if(position <= itemCount && position > 0)
  {
    Node* node = headPtr;
    for(int i = 1; i < position; i++)
    {
      node = node->getNext();
    }
    cout<< node->getValue()<<endl;
  }
  else
  {
    cout<<"Out of bounds"<<endl;
  }
}



void LinkedList::printReverse()
{
  for(int i = itemCount; i >= 1; i--)
  {
    printAt(i);
    cout << " "<<endl;
  }
}

int LinkedList::getVal(int val)
{
  Node* node = headPtr;
  for(int i=1;i<val;i++)
  {
     node=node->getNext();
  }
  return(node->getValue());
}
