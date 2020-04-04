Link: main.o LinkedList.o Node.o Executive.o
	g++ -std=c++11 -g -Wall main.o LinkedList.o  Node.o Executive.o -o Link

main.o: main.cpp Executive.h LinkedList.h
	g++ -std=c++11 -g -Wall -c main.cpp

Node.o: Node.h Node.cpp
	g++ -std=c++11 -g -Wall -c Node.cpp

LinkedList.o: LinkedList.h LinkedList.cpp
	g++ -std=c++11 -g -Wall -c LinkedList.cpp

Executive.o: Executive.h Executive.cpp
	g++ -std=c++11 -g -Wall -c Executive.cpp

clean:
	rm *.o Link
