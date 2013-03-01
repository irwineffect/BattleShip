#include "ServerFunct.h"

/**********************************************
*	startListening()
*
*	Description:
*		This function initializes the listening socket and begins listening for connections
*
*	input parameters:
*
*	returns:
*
*
*
**********************************************/
bool startListening(int PortNo, char* IPaddr, SOCKET *listener)
{

	SOCKADDR_IN socket_info;
	int bind_status;

	socket_info.sin_family = AF_INET;	//type of connection (use this)
	socket_info.sin_port = htons(PortNo);	//the port number. (htons() is a function to convert it to the proper format)
	//socket_info.sin_addr.s_addr = inet_addr(IPaddr); //only accept connections from specified IP address
	socket_info.sin_addr.s_addr = htonl (INADDR_ANY);  //accept connections from any IP address

	*listener = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);	//initial the socket for ipv4, and a TCP streaming socket
	if (*listener == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << endl;
		return false;
	}
	//else
		//cout << "Socket creation successful" << endl;
	
	bind_status = bind(*listener, (LPSOCKADDR) &socket_info, sizeof(socket_info));	//binds the socket to listen to a port
	
	if (bind_status == SOCKET_ERROR)
	{
		cout << "Socket binding failed" << endl;
		return false;
	}
	else
	{
		//cout << "Socket binding successful" << endl;
		cout << "Starting to listen...";
		listen(*listener, SOMAXCONN);	//causes the socket to start listening. SOMAXCONN is a system constant specifying the max number of connections possible at once
		cout << "listening..." << endl;
	}

	return true;
}


/**********************************************
*	exitPrompt()
*
*	Description:
*		Waits for the user to type "exit" at the command prompt. When this happens, it changes the
*		"run" variable to false, which begins the process of stopping all other threads.
*
*	input parameters:
*
*	returns:
*
*
*
**********************************************/
void exitPrompt(bool* run)
{
	string buffer;
	do
	{
		cout << "Type \"exit\" to stop server" << endl;

		cin >> buffer;

		if (buffer == "exit")
			*run = false;

	}while ((*run));	//the run variable is used by the other threads to know when to quit

	return;
}



/**********************************************
*	accepterLoop()
*
*	Description:
*		Accepts new connections. When a new client attempts to connect, gives it a new socket to comuunicate on,
*		then starts a new thread that talks on that socket. When the 'run' variable becomes false, it closes
*		all communication threads and then exits
*
*	input parameters:
*
*	returns:
*
*
*
**********************************************/
void accepterLoop(SOCKET mListenSocket, SOCKADDR listen_socket_info, int socket_size, bool *run)
{
	SOCKET tempSocket;
	SOCKET clientSockets[MAXCLIENTS];
	thread mClients[MAXCLIENTS];
	int numClients = 0;
	int numThreads = 0;

	u_long iMode = 1;
	ioctlsocket(mListenSocket, FIONBIO, &iMode);	//sets the listening socket to be nonblocking, so we can poll it for new connections

	while((*run))
	{
		tempSocket = NULL;
		/*	accept() is a normally a blocking function, meaning that program flow would halt here
			but because of the ioctlsocket() call up above, it makes the function non-blocking, so
			we need to use a polling method
		*/
		tempSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);	

		iMode = 0;
		//we want the socket that talks to be blocking
		ioctlsocket(tempSocket, FIONBIO, &iMode);	//sets the communication socket to be blocking, so that it doesn't act until it recieves anything

		if (tempSocket != INVALID_SOCKET)	//accept() will return an invalid socket if there is no pending connections
		{
			clientSockets[numClients] = tempSocket;	//add to array of clients sockets
			thread temp(talk, tempSocket, &numClients);	//start a thread for talking on the sockets
			mClients[numThreads].swap(temp);	//add that thread to the array of threads
			++numClients;
			++numThreads;
			cout << "client added!" << endl << numClients << " clients connected" << endl << endl;
		}

		//sleep_for(milliseconds(100));
	}

	cout << "trying to exit...waiting for all clients to disconnect..." << endl;
	
	for (int i=0; i < MAXCLIENTS; ++i)
	{
		closesocket(clientSockets[i]);	//closes all the sockets, which will in turn allow all communication threads to die
	}

	for (int i=0; i < numThreads; ++i)
	{
		mClients[i].join();	//ensures all the communication threads have been killed
	}

}


/**********************************************
*	talk()
*
*	Description:
*		Communicates on the socket. Currently just echos back whatever is sent to the server
*		
*
*	input parameters:
*
*	returns:
*
*
*
**********************************************/
void talk(SOCKET mSocket, int* numClients)
{
	char buffer[BUFSIZE] = "";	//data buffer
	int connected = 1;	

	while( connected != -1 )
	{
		//recv() is a blocking function, meaning program flow will halt here until
		//somebody sends something
		connected = recv(mSocket, buffer, BUFSIZE, 0);	//recv() will return a -1 if the socket is disconnected

		if (buffer[0] != NULL)
		{
			cout << "received: " << endl << buffer << endl;
			strcat(buffer, " -from server");
			send(mSocket, buffer, BUFSIZE, 0);	//send the data back that was recieved
			cout << "echoed!" << endl << endl;
		}

		buffer[0] = '\0';
	}

	cout << "client disconnected" << endl << endl;
	closesocket(mSocket);	//close the socket
	--(*numClients);
	cout << (*numClients) << " clients connected" << endl;

	return;
}