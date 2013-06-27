#ifndef NETFUNCT_H
#define NETFUNCT_H

//uncomment proper operating system
#define WINDOWS
//#define LINUX

#ifdef LINUX
#define SERVER_ONLY	//client is not ready for building in linux
#endif


#define _CRT_SECURE_NO_WARNINGS	//hides security warnings

//Libraries
#include <iostream>
#include <string.h>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>

#ifdef WINDOWS
	#include <winsock.h>
#elif defined(LINUX)
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <fcntl.h>
#endif

//System constants
#define SCK_VERSION1	0x0101
#define SCK_VERSION2	0x0202
#define BUFSIZE	128
#define MAXCLIENTS 10
#define IDLE_PERIOD 100 //time to sleep for on the accepting thread (this thread uses polling)
#define PORT 3410

#ifdef LINUX
	#define SOCKET int
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
#endif

//Using Statements
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::string;
using std::mutex;
using std::fstream;
using std::ofstream;
using std::ifstream;
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
	Msg* root;	//root node pointer
	mutex mWritelock;	//mutex used to lock critical sections of code

};

//BattleServer Class

typedef struct socketnode	//node for the client list
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
	void Accepter(SOCKET mListenSocket, sockaddr listen_socket_info, int socket_size);
	void AddClient(SOCKET newSocket);
	void Receiver(SOCKET mSocket, int Id);
	void Sender(void);
	void RemoveSocketNode(int Id);


	//Private Members
	SocketNode* root;
	MsgBuffer mBuffer;
	int numClients;
	int idCounter;
	bool run;
	mutex mWritelock;

};

#ifndef SERVER_ONLY
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
#endif





#endif