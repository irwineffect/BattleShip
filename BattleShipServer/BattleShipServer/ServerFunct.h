#ifndef SERVERFUNCT_H
#define SERVERFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>
#include <string.h>
#include <thread>
#include <string>

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


bool ConnectToHost( int PortNo, char* IPaddr);
bool startListening(int PortNo, char* IPaddr, SOCKET *listener);
void talk(SOCKET mSocket, int* numClients);
void CloseConnection(void);
void exitPrompt(bool* run);
void accepterLoop(SOCKET mListenSocket, SOCKADDR listen_socket_info, int socket_size, bool *run);


#endif