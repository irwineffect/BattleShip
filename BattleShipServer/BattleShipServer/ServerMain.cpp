#include "ServerFunct.h"


int main (void)
{
	int mPort = 3410; //listening port
	char mIpAddr[16] = "127.0.0.1"; //self IP iddress
	SOCKET mListenSocket = NULL, tempSocket = NULL; //socket initialization
	thread mClients[MAXCLIENTS]; //thread initialization
	int numClients = 0;	//current number of connected clients
	int numThreads = 0; //current number of running threads
	WSADATA wsadata; //variable used for initializing the socket software stuff
	SOCKADDR listen_socket_info;	//variable used for inititalizing the listening socket
	int socket_size = sizeof(listen_socket_info);	//size of the listening socket, used for initialization
	char buffer[BUFSIZE] = "";	//communication buffer


	cout << "Starting Server" << endl;

	if (WSAStartup(SCK_VERSION1, &wsadata))		//initialize socket stuff
		cout << "WSAStartup failed" << endl;
	//else
		//cout << "WSAStartup successful" << endl;

	if (startListening(mPort, mIpAddr, &mListenSocket))	//startListening() returns successful if all initialization things went well
	{	
		bool run = true;
		thread exiter(exitPrompt, &run);	//this thread waits for the "exit" command to be entered on the terminal. 
		thread acceptThread(accepterLoop, mListenSocket, listen_socket_info, socket_size, &run); //this thread accepts new clients (which then starts a talking thread for the new client)

		exiter.join(); //wait for the exit command to be entered on the terminal
		acceptThread.join(); //wait for all connections and other threads to end

		closesocket(mListenSocket); //close the listening socket
	}

	cout << "Done!" << endl;
	//system("pause");

	WSACleanup();	//not sure exactly what this does, but we're always supposed to call this at the end (I think it closes sockets, etc) 

	return 0;
}
