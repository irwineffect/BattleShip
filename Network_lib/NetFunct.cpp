#include "NetFunct.h"

/***************************************************************************************************************
****************************************************************************************************************
*	MsgBuffer Class
*
*
*
*
*
*
*
*
*
***************************************************************************************************************/
MsgBuffer::MsgBuffer(void)
{
	this->root = NULL;

	return;
}

MsgBuffer::~MsgBuffer(void)
{
	deconstructor(this->root);
}

void MsgBuffer::deconstructor(Msg* node)
{
	if (node != NULL)
	{
		deconstructor(node->next);
		delete node;
	}
}

void MsgBuffer::queue(char input[BUFSIZE])
{
	Msg* walker;

	mWritelock.lock();	//begin critical section of code

	if (this->root != NULL)
	{
		walker = this->root;
		//walks through the queue to the end
		while(walker->next!= NULL)
			walker = walker->next;
		walker->next = new Msg;
		walker = walker->next;
	}
	else
	{
		root = new Msg;
		walker = this->root;
	}


	walker->next = NULL;	//null the pointer for the last message

	//copy the input message to the new Msg element
	for (int i=0; i < BUFSIZE; ++i)
		walker->message[i] = input[i];

	mWritelock.unlock();	//end critical section of code

	return;
}

void MsgBuffer::dequeue(char output[BUFSIZE])
{
	Msg* temp = NULL;

	mWritelock.lock();	//begin critical section of code

	temp = this->root;

	if (temp != NULL)	//check to make sure the queue isn't empty
	{
		root = root->next;	//move the root node

		//copy the Msg to the output array
		for (int i=0; i < BUFSIZE; ++i)
			output[i] = temp->message[i];

		delete temp;	//delete the Msg node
	}
	else
	{
		output[0] = '\0';
		//cout << "Error: queue is empty!" << endl;
	}

	mWritelock.unlock();	//end critical section of code

	return;
}



/***************************************************************************************************************
****************************************************************************************************************
*	CommServer Class
*
*
*
*
*
*
*
*
*
***************************************************************************************************************/
CommServer::CommServer(void)
{
	this->root = NULL;
	this->numClients = 0;
	this->run = false;
	this->idCounter = 0;

	return;
}

CommServer::~CommServer(void)
{
	Deconstructor(this->root);
}

void CommServer::Deconstructor(SocketNode* node)
{
	if (node != NULL)
	{
		Deconstructor(node->next);
		delete node;
	}
}



void CommServer::Start(void)
{
	int mPort = PORT; //listening port
	char mIpAddr[16] = "127.0.0.1"; //self IP iddress
	SOCKET mListenSocket = NULL, tempSocket = NULL; //socket initialization
	sockaddr listen_socket_info;	//variable used for inititalizing the listening socket
	int socket_size = sizeof(listen_socket_info);	//size of the listening socket, used for initialization

#ifdef WINDOWS
	WSADATA wsadata; //variable used for initializing the socket software stuff
#endif

	cout << "Starting Server" << endl;

#ifdef WINDOWS
	if (WSAStartup(SCK_VERSION1, &wsadata))		//initialize socket stuff
		cout << "WSAStartup failed" << endl;
	//else
	//cout << "WSAStartup successful" << endl;
#endif

	if (StartListening(mPort, mIpAddr, mListenSocket))	//startListening() returns successful if all initialization things went well
	{	
		run = true;
		thread exiterThread(&CommServer::ExitPrompt, this);	//this thread waits for the "exit" command to be entered on the terminal. 
		thread accepterThread(&CommServer::Accepter, this, mListenSocket, listen_socket_info, socket_size); //this thread accepts new clients (which then starts a talking thread for the new client)
		thread senderThread(&CommServer::Sender, this);	//this thread processes the message queue and sends it to everyone

		exiterThread.join(); //wait for the exit command to be entered on the terminal
		accepterThread.join(); //wait for all connections and other threads to end
		senderThread.join();

#ifdef WINDOWS
		closesocket(mListenSocket); //close the listening socket
#elif defined (LINUX)
		close (mListenSocket);
#endif
	}

	cout << "Done!" << endl;
	//system("pause");

#ifdef WINDOWS
	WSACleanup();	//not sure exactly what this does, but we're always supposed to call this at the end (I think it closes sockets, etc) 
#endif
}

bool CommServer::StartListening(int PortNo, char* IPaddr, SOCKET &listener)
{

#ifdef WINDOWS
	SOCKADDR_IN socket_info;
#elif defined (LINUX)
	struct sockaddr_in socket_info;
#endif

	int bind_status;

	socket_info.sin_family = AF_INET;	//type of connection (use this)
	socket_info.sin_port = htons(PortNo);	//the port number. (htons() is a function to convert it to the proper format)
	//socket_info.sin_addr.s_addr = inet_addr(IPaddr); //only accept connections from specified IP address
	socket_info.sin_addr.s_addr = htonl (INADDR_ANY);  //accept connections from any IP address

	listener = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);	//initial the socket for ipv4, and a TCP streaming socket
	if (listener == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << endl;
		return false;
	}
	//else
	//cout << "Socket creation successful" << endl;

#ifdef WINDOWS
	bind_status = bind(listener, (LPSOCKADDR) &socket_info, sizeof(socket_info));	//binds the socket to listen to a port
#elif defined (LINUX)
	bind_status = bind(listener, (const sockaddr*) &socket_info, sizeof(socket_info));	//binds the socket to listen to a port
#endif
	if (bind_status == SOCKET_ERROR)
	{
		cout << "Socket binding failed" << endl;
		return false;
	}
	else
	{
		//cout << "Socket binding successful" << endl;
		cout << "Starting to listen...";
		listen(listener, SOMAXCONN);	//causes the socket to start listening. SOMAXCONN is a system constant specifying the max number of connections possible at once
		cout << "listening..." << endl;
	}

	return true;
}


void CommServer::ExitPrompt(void)
{
	string buffer;
	do
	{
		cout << "Type \"exit\" to stop server" << endl;

		cin >> buffer;

		if (buffer == "exit")
			run = false;

	}while (run);	//the run variable is used by the other threads to know when to quit

	return;
}

void CommServer::AddClient(SOCKET newSocket)
{
	SocketNode* walker = NULL;

	mWritelock.lock();	//begin critical section of code

	if (this->root != NULL)
	{
		walker = this->root;
		//walks through the queue to the end
		while(walker->next!= NULL)
			walker = walker->next;
		walker->next = new SocketNode;
		walker = walker->next;
	}
	else
	{
		root = new SocketNode;
		walker = this->root;
	}

	walker->next = NULL;	//null the pointer for the last node

	//add new socket and thread and stuff

	walker->mSocket = newSocket;
	thread temp(&CommServer::Receiver, this, walker->mSocket, idCounter);	//start a thread for listening to the sockets
	walker->mThread.swap(temp);	//attaches thread handle to the node
	walker->mId = this->idCounter;
	++(this->idCounter);

	mWritelock.unlock();	//end critical section of code

	return;
}

void CommServer::Accepter(SOCKET mListenSocket, sockaddr listen_socket_info, int socket_size)
{
	SOCKET tempSocket;

#ifdef WINDOWS
	u_long iMode = 1;
	ioctlsocket(mListenSocket, FIONBIO, &iMode);	//sets the listening socket to be nonblocking, so we can poll it for new connections
#elif defined (LINUX)
	fcntl(mListenSocket, F_SETFL, O_NONBLOCK);
#endif

	while(run)
	{
		tempSocket = NULL;
		/*	accept() is a normally a blocking function, meaning that program flow would halt here
		but because of the ioctlsocket()/fcntl() call up above, it makes the function non-blocking, so
		we need to use a polling method
		*/

#ifdef WINDOWS
		tempSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);	
		iMode = 0;	//we want the socket that talks to be blocking 
		ioctlsocket(tempSocket, FIONBIO, &iMode);	//sets the communication socket to be blocking
#elif defined (LINUX)
		tempSocket = accept(mListenSocket, (struct sockaddr*) &listen_socket_info, (socklen_t*) &socket_size);	
#endif
		if (tempSocket != INVALID_SOCKET)	//accept() will return an invalid socket if there is no pending connections
		{
			AddClient(tempSocket);
			++numClients;
			cout << endl << "client added!" << endl << numClients << " clients connected" << endl;
		}

		sleep_for(milliseconds(IDLE_PERIOD));
	}

	cout << "trying to exit...waiting for all clients to disconnect..." << endl;

	mWritelock.lock();	//begin critical section of code

	SocketNode* walker = root;

	while (walker != NULL)
	{
#ifdef WINDOWS
		closesocket(walker->mSocket);	//close all of the socket connections
#elif defined (LINUX)
		close(walker->mSocket);
#endif		
		walker = walker->next;
	}

	walker = root;
	while (walker != NULL)
	{
		walker->mThread.join(); //ensures all the communication threads have been killed
		walker = walker->next;
	}

	mWritelock.unlock();	//end critical section of code

	return;
}

void CommServer::Receiver(SOCKET mSocket, int Id)
{
	char buffer[BUFSIZE] = "";	//data buffer
	int connected = 1;	

	char identifier[32] = "";
	sprintf(identifier, " -from client %d", Id);

	while( connected != -1 )
	{

		for (int i=0; i < BUFSIZE; ++i)	//clear the buffer
		{
			buffer[i] = '\0';
		}
		//recv() is a blocking function, meaning program flow will halt here until
		//somebody sends something
		connected = recv(mSocket, buffer, BUFSIZE, 0);	//recv() will return a -1 if the socket is disconnected

		if (buffer[0] != NULL)
		{
			cout << endl << "received: " << endl << buffer << endl;
			//strcat(buffer, identifier);
			this->mBuffer.queue(buffer);
			//send(mSocket, buffer, BUFSIZE, 0);	//send the data back that was recieved
			//cout << "echoed!" << endl << endl;
		}
	}	

	if (this->run) //server is still running, so remove node from the list
	{
		cout << endl << "client disconnected" << endl;

#ifdef WINDOWS
		closesocket(mSocket);	//close the socket
#elif defined (LINUX)
		close(mSocket);
#endif

		RemoveSocketNode(Id);
		--numClients;
		cout << numClients << " clients connected" << endl;
	}//else: server is shutting down, so it will destroy the list by itself

	return;
}


void CommServer::Sender(void)
{
	char message[BUFSIZE] = {'\0'};
	SocketNode* walker;
	int message_size = 0;

	while(run)
	{
		walker = this->root;	//start at the beginning of the list of sockets
		this->mBuffer.dequeue(message); //pull a message out of the queue
		if (message[0] != '\0')	//check to see if there are any messages in the queue
		{
			while(walker != NULL)	//goes through the queue, sends the message to everyone.
			{
				send(walker->mSocket, message, BUFSIZE, 0);
				walker = walker->next;
			}
		}
		else
			sleep_for(milliseconds(IDLE_PERIOD)); //if there are no messages in the queue, chill for a bit

	}
}


void CommServer::RemoveSocketNode(int Id)
{
	SocketNode* walker = NULL;
	bool foundnode = false;

	mWritelock.lock();	//begin critical section of code

	walker = this->root;

	if (walker == NULL)	//should not happen, but good to check
	{
		cerr << "Error: cannot remove ClientNode, root is null" << endl;
	}
	else if (walker->mId == Id) //special case where root node is the node to remove
	{
		foundnode = true;
		root = walker->next;
		walker->mThread.detach();
		delete walker;
	}
	else
	{
		while(walker->next != NULL)
		{
			if (walker->next->mId == Id)
			{
				foundnode = true;
				SocketNode* tempnode = walker->next;
				walker->next = walker->next->next;
				tempnode->mThread.detach();
				delete tempnode;
				break;
			}
			else
			{
				walker = walker->next;
			}
		}
	}

	if (!foundnode) //this will be the case if we don't find the node
	{
		cerr << "Error: Did not delete SocketNode, could not find node " << Id << endl;
	}


	mWritelock.unlock();	//end critical section of code

	return;
}


#ifndef SERVER_ONLY
/***************************************************************************************************************
****************************************************************************************************************
*	CommClient Class
*
*
*
*
*
*
*
*
*
***************************************************************************************************************/
CommClient::CommClient(string filename)
{
	WSADATA wsadata; //variable used for initializing the socket software stuff
	ifstream cfgfile_in;

	WSAStartup(SCK_VERSION2, &wsadata);	//initialize socket stuff

	cfgfile_in.open(filename);

	if (cfgfile_in.is_open() && ParseFile(cfgfile_in) )
	{
		cout << "Config File loaded successfully!" << endl;
		cfgfile_in.close();
	}
	else
	{
		ofstream cfgfile_out;
		cout << "Could not open configuration file, please input values manually" << endl << endl;
		cout << "Input hostname> ";
		cin >> mHostname;
		cout << "Input port> ";
		cin >> mPort;

		cfgfile_out.open(filename);
		cfgfile_out << "hostname:" << mHostname << endl;
		cfgfile_out << "port:" << mPort << endl;
		cfgfile_out.close();
	}



	if(mHostname[0] == '0') //if the null hostname is specified
	{
		this->socket_info.sin_addr.s_addr = inet_addr("127.0.0.1"); //use the loopback address

	}
	else //a hostname is given
	{
		struct hostent *remoteHost = NULL;
		remoteHost = gethostbyname(mHostname.c_str()); //attempt DNS lookup

		if( remoteHost == NULL)
		{
			char mIpAddr[32] = "";
			cout << "Could not resolve hostname, please enter ip address manually" << endl;
			cin >> mIpAddr;
			this->socket_info.sin_addr.s_addr = inet_addr(mIpAddr);
		}
		else
		{
			this->socket_info.sin_addr.s_addr = *(u_long*)remoteHost->h_addr_list[0];
		}
	}

	this->run = true;
	this->socket_info.sin_family = AF_INET;	//use ipv4
	this->socket_info.sin_port = htons(mPort);	//use selected port
}
CommClient::~CommClient(void)
{

	WSACleanup();	//not sure exactly what this does, but we're always supposed to call this at the end (I think it closes sockets, etc) 
}

bool CommClient::Start(void)
{
	int connect_status;

	cout << "Starting Client" << endl;	

	this->mSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);	//initialze the socket to use ipv4, tcp streaming
	if (this->mSocket == INVALID_SOCKET)
		cout << "Socket creation failed" << endl;
	else
		cout << "Socket creation successful" << endl;

	//attempt to connect to the server
	//this will fail if no server is listening
	connect_status = connect(this->mSocket, (SOCKADDR*) &socket_info, sizeof(socket_info));	

	if (connect_status == SOCKET_ERROR)
	{
		cout << "Socket connection failed" << endl;
		return false;
	}
	else
	{
		cout << "Socket connection successful" << endl;
		thread temp(&CommClient::Receiver, this, mSocket);
		mReciever_thread.swap(temp);
		return true;
	}

}
void CommClient::End(void)
{
	if (mReciever_thread.joinable())
	{
		mReciever_thread.join();
	}

	closesocket(mSocket);	//close the socket

}

bool CommClient::SendMsg(string message)
{
	int sent_count     = 0,
		message_length = 0;
	char cstr_message[BUFSIZE] = "";



	message_length = message.length();


	if (message_length >= BUFSIZE)	//if the message is too large
	{
		return false;	//fail the command
	}

	for (int i=0; i < message_length; ++i) //copy the message into a c-style string
	{
		cstr_message[i] = message[i];
	}

	cstr_message[message_length] = '\0'; //ensure the c-style string is null-terminated


	sent_count = send(mSocket, cstr_message, message_length, 0);	//send the message to the server

	if (sent_count != message_length)	//we did not send the whole message
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CommClient::Receiver(SOCKET mSocket)
{
	char buffer[BUFSIZE] = "";	//data buffer
	int connected = 1;	

	//TEST mRecv;

	while( connected != -1  && run==true)
	{
		//recv() is a blocking function, meaning program flow will halt here until
		//somebody sends something
		connected = recv(mSocket, buffer, BUFSIZE, 0);	//recv() will return a -1 if the socket is disconnected		

		if (buffer[0] != NULL)
		{
			cout << endl << "received: " << endl << buffer << endl << endl;
		}

		buffer[0] = '\0';
	}	

	cout << "disconnected" << endl << endl;
	closesocket(mSocket);	//close the socket

	return;
}


bool CommClient::ParseFile(ifstream &cfgfile)
{
	string buffer;
	string parameter;
	string value;
	int position;

	//initialize values
	mHostname = "";
	mPort = 0;

	while ( cfgfile.good() )
	{
		getline(cfgfile, buffer);
		position = buffer.find(':');
		parameter = buffer.substr( 0, position );
		value = buffer.substr( position+1, buffer.length() );

		if (parameter == "hostname")
		{
			mHostname = value;
		}
		else if (parameter == "port")
		{
			mPort = atoi( value.c_str() );
		}
	}

	if (mHostname == "" || mPort == 0)
		return false;
	else
		return true;

}

#endif
