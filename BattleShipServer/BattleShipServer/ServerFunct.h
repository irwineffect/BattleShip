#ifndef SERVERFUNCT_H
#define SERVERFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>


//Using Statements
//using std::cin;
//using std::cout;
//using std::endl;
using namespace std;


bool ConnectToHost( int PortNo, char* IPaddr);
void CloseConnection(void);


#endif