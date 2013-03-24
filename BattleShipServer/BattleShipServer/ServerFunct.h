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
#define IDLE_PERIOD 100 //time to sleep for on the accepting thread (this thread uses polling)


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
bool startListening(int PortNo, char* IPaddr, SOCKET &listener);
void exitPrompt(bool& run);
void accepterLoop(SOCKET mListenSocket, SOCKADDR listen_socket_info, int socket_size, bool& run);
void receiver(SOCKET mSocket, int *numClients, char outbuffer[BUFSIZE], bool *canWrite);
//void sender(SOCKET mSocket, bool &run, char outbuffer[BUFSIZE], bool &canWrite);
//void cleanup(

//array management
//void cleanArray(SOCKET Clients[MAXCLIENTS], int* numClients);
//void cleanArray(int Clients[MAXCLIENTS], int* numClients);



//Msgbuffer Class

typedef struct msg	//node for the buffer
{
	char message[BUFSIZE];
	struct msg* next;

} Msg;

class MsgBuffer	//the class
{
public:

		MsgBuffer::MsgBuffer(void);	//constructor
		MsgBuffer::~MsgBuffer(void); //destructor
	void MsgBuffer::queue(char input[BUFSIZE]);
	void MsgBuffer::dequeue(char output[BUFSIZE]);
	

private:
	void MsgBuffer::deconstructor(Msg* node);	//destructor helper function
	Msg* root;

};

//BattleServer Class

typedef struct socketnode	//node for the buffer
{
	SOCKET mSocket;
	thread mThread;
	int mId;
	struct socketnode* next;

} SocketNode;

class BattleServer	//the class
{
public:
		//Public Methods	
		BattleServer::BattleServer(void);	//constructor
		BattleServer::~BattleServer(void); //destructor
		void BattleServer::Start(void);
	

private:
	//Private Methods
	void BattleServer::Deconstructor(SocketNode* node);	//destructor helper function
	bool BattleServer::StartListening(int PortNo, char* IPaddr, SOCKET &listener);
	void BattleServer::ExitPrompt(void);
	void BattleServer::Accepter(SOCKET mListenSocket, SOCKADDR listen_socket_info, int socket_size);
	void BattleServer::AddClient(SOCKET newSocket);
	void BattleServer::Receiver(SOCKET mSocket, int Id);
	void BattleServer::Sender(void);


	//Private Members
	SocketNode* root;
	MsgBuffer mBuffer;
	int numClients;
	int idCounter;
	bool run;
	bool writelock;

};

#endif