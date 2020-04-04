

#ifndef EXECUTIVE_H
#define EXECUTIVE_H

#include <iostream>
#include<string>
#include "LinkedList.h"
#include <fstream>
using namespace std;

class Executive
{
	public:
	/**
	*@pre none
	*@post runs the stuff the user is going to see
	**/
	void run(string fileName);
	void menu();

};
#endif
