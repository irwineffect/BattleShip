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
	CommClient(int mPort = 3410, char mHostname[128] = "");
	~CommClient();
	void Start();


private:
	void Receiver(SOCKET mSocket);
	void LoadConfig(string filename = "network_config.cfg");

//class members
	bool run;
	SOCKADDR_IN socket_info;
	SOCKET mSocket;

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