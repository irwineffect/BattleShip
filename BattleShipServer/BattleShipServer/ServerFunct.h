#ifndef SERVERFUNCT_H
#define SERVERFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>
#include <string.h>
#include <thread>
#include <string>

//System constants
#define SCK_VERSION1	0x0101
#define SCK_VERSION2	0x0202
#define BUFSIZE	128
#define MAXCLIENTS 10


//Using Statements
using std::cin;
using std::cout;
using std::endl;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::string;
//using namespace std;

//Function Headers
bool startListening(int PortNo, char* IPaddr, SOCKET *listener);
void exitPrompt(bool* run);
void accepterLoop(SOCKET mListenSocket, SOCKADDR listen_socket_info, int socket_size, bool *run);
void talk(SOCKET mSocket, int* numClients);


#endif