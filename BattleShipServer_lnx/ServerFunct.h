#ifndef SERVERFUNCT_H
#define SERVERFUNCT_H

//Libraries
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <thread>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

//System constants
#define SCK_VERSION1	0x0101
#define SCK_VERSION2	0x0202
#define BUFSIZE	128
#define MAXCLIENTS 10
#define IDLE_PERIOD 100 //time to sleep for on the accepting thread (this thread uses polling)
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 3410 
#define closesocket close

//Using Statements
using std::cin;
using std::cout;
using std::endl;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::string;
//using namespace std;



//Msgbuffer Class

typedef struct msg	//node for the buffer
{
	char message[BUFSIZE];
	struct msg* next;

} Msg;

class MsgBuffer	//the class
{
public:

		MsgBuffer(void);	//constructor
		~MsgBuffer(void); //destructor
	void queue(char input[BUFSIZE]);
	void dequeue(char output[BUFSIZE]);
	

private:
	void deconstructor(Msg* node);	//destructor helper function
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
		BattleServer(void);	//constructor
		~BattleServer(void); //destructor
		void Start(void);
	

private:
	//Private Methods
	void Deconstructor(SocketNode* node);	//destructor helper function
	bool StartListening(int PortNo, char* IPaddr, SOCKET &listener);
	void ExitPrompt(void);
	void Accepter(SOCKET mListenSocket, struct sockaddr listen_socket_info, int socket_size);
	void AddClient(SOCKET newSocket);
	void Receiver(SOCKET mSocket, int Id);
	void Sender(void);


	//Private Members
	SocketNode* root;
	MsgBuffer mBuffer;
	int numClients;
	int idCounter;
	bool run;
	bool writelock;

};

#endif
