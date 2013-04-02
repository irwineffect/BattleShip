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


void Receiver(SOCKET mSocket);

class CommClient
{
public:
	CommClient::CommClient(int mPort = 80, char mIpAddr[32] = "76.178.149.238");
	CommClient::~CommClient();
	void CommClient::Start();


private:
	void CommClient::Receiver(SOCKET mSocket);


	bool run;
	SOCKADDR_IN socket_info;
	SOCKET mSocket;






};

#endif