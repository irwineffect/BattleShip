#ifndef CLIENTFUNCT_H
#define CLIENTFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>
#include <thread>



//Using Statements
using std::cin;
using std::cout;
using std::endl;
using std::thread;
//using namespace std;

#define SCK_VERSION2 0x0202
#define BUFSIZE 128



class CommClient
{
public:
	CommClient::CommClient(int mPort = 3410, char mHostname[128] = "");
	CommClient::~CommClient();
	void CommClient::Start();


private:
	void CommClient::Receiver(SOCKET mSocket);


	bool run;
	SOCKADDR_IN socket_info;
	SOCKET mSocket;

};

#endif