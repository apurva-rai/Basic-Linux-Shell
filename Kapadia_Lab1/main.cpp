#include "Executive.h"
#include<iostream>
#include<string>

int main(int argc, char* argv[])
{
std::string fileName;
if(argc < 2)
{
	cout << "Incorrect number of parameters!" << endl;

}
else
{
	fileName = (argv[1]);
	Executive exec;
	exec.run(fileName);

}
return(0);
}
