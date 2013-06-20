#ifndef CLIENTFUNCT_H
#define CLIENTFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>
#include <thread>
#include <string>
#include <fstream>


//Using Statements
using std::cin;
using std::cout;
using std::endl;
using std::thread;
using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
//using namespace std;

#define SCK_VERSION2 0x0202
#define BUFSIZE 128

class CommClient
{
public:
	CommClient(string filename = "network_config.cfg");
	~CommClient(void);
	bool Start(void);
	void End(void);
	bool SendMsg(string message);



private:
	void Receiver(SOCKET mSocket);
	bool ParseFile(ifstream &cfgfile);

//class members
	bool run;
	SOCKADDR_IN socket_info;
	SOCKET mSocket;
	string mHostname;
	int mPort;
	thread mReciever_thread;

};


class GameBoard
{
public:
	GameBoard(int size);
	~GameBoard();
	void Display(void);

private:
	
	
//class members	
	int mArraysize;
	char** mArray;
};

#endif