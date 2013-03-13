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
void receiver(SOCKET mSocket, int* numClients, char outbuffer[BUFSIZE], bool* canWrite);
void sender(SOCKET mSocket, bool *run, char outbuffer[BUFSIZE], bool*canWrite);
//void cleanup(

//array management
void cleanArray(SOCKET Clients[MAXCLIENTS], int* numClients);
void cleanArray(int Clients[MAXCLIENTS], int* numClients);



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

#endif