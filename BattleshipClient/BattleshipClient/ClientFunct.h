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
#define BUF_SIZE 128

//typedef struct {
//	int i;
//	char msg[12];
//	double j; 
//
//} TEST;


//void Receiver(SOCKET mSocket);

class CommClient
{
public:
	CommClient::CommClient(int mPort = 80, char mHostname[128] = "irwineffect.dyndns.org");
	CommClient::~CommClient();
	void CommClient::Start();


private:
	void CommClient::Receiver(SOCKET mSocket);


	bool run;
	SOCKADDR_IN socket_info;
	SOCKET mSocket;

};

#endif