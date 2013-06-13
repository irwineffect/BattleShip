#include "ClientFunct.h"

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
CommClient::CommClient(int mPort, char mHostname[128])
{
	WSADATA wsadata; //variable used for initializing the socket software stuff
	
	WSAStartup(SCK_VERSION2, &wsadata);	//initialize socket stuff

	if(mHostname[0] == '\0') //if the null hostname is specified
	{
		this->socket_info.sin_addr.s_addr = inet_addr("127.0.0.1"); //use the loopback address

	}
	else //a hostname is given
	{
		struct hostent *remoteHost = NULL;
		remoteHost = gethostbyname(mHostname); //attempt DNS lookup

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
CommClient::~CommClient()
{
}

void CommClient::Start()
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
		cout << "Socket connection failed" << endl;
	else
	{
		cout << "Socket connection successful" << endl;
		thread temp(&CommClient::Receiver, this, mSocket);
		int connected = 1;
		cout << endl << endl;

		char buffer[BUFSIZE] = "";	//data buffer
		while (connected != -1)
		{
			cin.getline(buffer, BUFSIZE, '\n');	//read data from the user
			//fflush(stdin);
			//cin.clear();
			buffer[BUFSIZE-1] = '\0'; //prevent sending too much data
			
			connected =	send(mSocket, buffer, BUFSIZE, 0);	//send the data to the server

			buffer[0] = '\0';
		}
		temp.join();

	}

	cout << endl;
//	system("pause");

	cout << "Disconnected" << endl;



	closesocket(mSocket);	//close the socket
	WSACleanup();	//not sure exactly what this does, but we're always supposed to call this at the end (I think it closes sockets, etc) 


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





/***************************************************************************************************************
****************************************************************************************************************
*	GameBoard Class
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
GameBoard::GameBoard(int size)
{
	int i,j;

	if (size > 26)	//limit the size of the array
		size = 26;

	mArraysize = size;

	//create the array.
	mArray = new char*[size];

	for (i=0; i<size; ++i)
		mArray[i] = new char[size];

	for (i=0; i<size; ++i)
		for(j=0; j<size; ++j)
			mArray[i][j] = '-';
	
	return;
}

GameBoard::~GameBoard()
{
	for (int i=0; i<mArraysize; ++i)
		delete[] mArray[i];
	delete[] mArray;

	return;
}

void GameBoard::Display(void)
{
	int i,j;

	cout << "   ";
	for (i=0; i<mArraysize; ++i)
		cout << (char)('A'+i) << ' ';
	cout << endl;

	for (i=0; i<mArraysize; ++i)
	{
		if (i>9)
			cout << i << ' ';
		else
			cout << ' ' << i << ' ';

		for (j=0; j<mArraysize; ++j)
			cout << mArray[i][j] << ' ';
		cout << endl;
	}

	return;
}