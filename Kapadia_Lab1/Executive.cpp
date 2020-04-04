


#include "Executive.h"
#include <iostream>
#include<string>
#include "LinkedList.h"
#include <fstream>
#include <limits>

using namespace std;

void Executive::menu()
{
	cout<<endl;
	cout << "Choose one operation from the options below: " << endl;
	cout << "" << endl;
	cout << "1. Is Empty" << endl;
	cout << "2. Length" << endl;
	cout << "3. Insert" << endl;
	cout << "4. Delete" << endl;
	cout << "5. Delete Duplicates" << endl;
	cout << "6. Find" << endl;
	cout << "7. Find Next" << endl;
	cout << "8. Print" << endl;
	cout << "9. ReverseList" << endl;
	cout << "10. Print At" << endl;
	cout << "11. Exit" << endl;
	cout<<endl;
}


void Executive::run(string fileName)
{
	LinkedList list;
	int selection;
	ifstream file(fileName);
 	if(!file)
	{
		 cout << "File does not exist or could not be opened" << endl;
		 //break;
	}
	while(true)
	{
        int x;
        file>>x;
        if(file.eof())
        break;
        list.insert(x);
    }

		bool running = true;

		while(running){
		menu();
		cin >> selection;
		while(cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout<<"Please enter a numeric (int) value"<<endl;
			cin>>selection;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if(selection == 1)
		{
				if(list.isEmpty())
						cout << "The list is empty" << endl;

				else
						cout << "The list is not empty" << endl;
				//break;
				//break;
		}
		else if(selection == 2)
		{
				cout <<"The length of the list is: "<<list.getLength()<<endl;
				//break;
		}
		else if(selection == 3)
		{
				int insertV;
				cout << "Choose a number that will be inserted:" << endl;
				cin >> insertV;
				cout << "" << endl;
				list.insert(insertV);
				cout << insertV << " has been inserted into the list.";
				//break;
		}
		else if(selection == 4)
		{
				int deleteV;
				cout << "Choose a number to be deleted from the list:" << endl;
				cin >> deleteV;
				list.deleteFunc(deleteV);


				//break;
		}
		else if(selection == 5)
		{
				for(int i=1;i<=list.getLength();i++)
				{
	 				list.removeDup(list.getVal(i));
 				}
 				cout<<"Duplicates deleted."<<endl;
				//break;
		}
		else if(selection == 6)
		{
				int findV;
				cout << "Enter the number to find:" << endl;
				cin >> findV;
				if(list.Find(findV))
						cout << findV << " is found in the list";
				else
						cout << findV << " is not found in the list";
				//break;
		}
		else if(selection == 7)
		{
				int findV;
				cout << "Enter the number to find its next element:" << endl;
				cin >> findV;
				list.FindNext(findV);
		}
		else if(selection == 8)
		{
				cout << "List: ";
				list.print();
				//break;
		}
		else if(selection == 9)
		{
				cout << "Reversed list: ";
				list.printReverse();
				//break;
		}
		else if(selection == 10)
		{
				int positionV;
				cout << "Choose a position to print element: " << endl;
				cin >> positionV;
				cout << "" << endl;
				cout << "The element at position " << positionV << " is: ";
				list.printAt(positionV);
				//break;
		}
		else if(selection == 11)
		{
				running = false;
				//break;
		}

	}
		file.close();
}
