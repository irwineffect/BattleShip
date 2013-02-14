#ifndef SERVERFUNCT_H
#define SERVERFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>

#define SCK_VERSION1	0x0101
#define SCK_VERSION2	0x0202


//Using Statements
//using std::cin;
//using std::cout;
//using std::endl;
using namespace std;


bool ConnectToHost( int PortNo, char* IPaddr);
void CloseConnection(void);


#endif