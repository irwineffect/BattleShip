#include "ClientFunct.h"




void Receiver(SOCKET mSocket)
{
	char buffer[BUF_SIZE] = "";	//data buffer
	int connected = 1;	

	while( connected != -1 )
	{
		//recv() is a blocking function, meaning program flow will halt here until
		//somebody sends something
		connected = recv(mSocket, buffer, BUF_SIZE, 0);	//recv() will return a -1 if the socket is disconnected

		if (buffer[0] != NULL)
		{
			cout << "received: " << endl << buffer << endl << endl;
		}

		buffer[0] = '\0';
	}	

	cout << "disconnected" << endl << endl;
	closesocket(mSocket);	//close the socket

	return;
}






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
CommClient::CommClient(int mPort, char mIpAddr[32])
{
	run = true;
	socket_info.sin_family = AF_INET;	//use ipv4
	socket_info.sin_port = htons(mPort);	//use selected port
	socket_info.sin_addr.s_addr = inet_addr(mIpAddr);	//use selected address
}
CommClient::~CommClient()
{
}

void CommClient::Start()
{
	int connect_status;
	WSADATA wsadata; //variable used for initializing the socket software stuff
	
	cout << "Starting Client" << endl;	
	
	WSAStartup(SCK_VERSION2, &wsadata);	//initialize socket stuff

	mSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);	//initialze the socket to use ipv4, tcp streaming
	if (mSocket == INVALID_SOCKET)
		cout << "Socket creation failed" << endl;
	else
		cout << "Socket creation successful" << endl;

	//attempt to connect to the server
	//this will fail if no server is listening
	connect_status = connect(mSocket, (SOCKADDR*) &socket_info, sizeof(socket_info));	
	if (connect_status == SOCKET_ERROR)
		cout << "Socket connection failed" << endl;
	else
	{
		cout << "Socket connection successful" << endl;
		thread temp(&CommClient::Receiver, this, mSocket);
		int connected = 1;
		cout << endl << endl;
		while (1)
		{
//			cin.getline(buffer, BUF_SIZE, '\n');	//read data from the user
//			connected =	send(mSocket, buffer, BUF_SIZE, 0);	//send the data to the server
		//	cout << "Sent: " << endl << buffer << endl;

			//buffer[0] = '\0';
		
			if (connected == -1)	//if the socket is disconnected
				break;
		}
		temp.join();
		
	}
	
	cout << endl;
	system("pause");

	cout << "Disconnected" << endl;


	
	closesocket(mSocket);	//close the socket
	WSACleanup();	//not sure exactly what this does, but we're always supposed to call this at the end (I think it closes sockets, etc) 
	

}

void CommClient::Receiver(SOCKET mSocket)
{
	char buffer[BUF_SIZE] = "";	//data buffer
	int connected = 1;	

	while( connected != -1  && run==true)
	{
		//recv() is a blocking function, meaning program flow will halt here until
		//somebody sends something
		connected = recv(mSocket, buffer, BUF_SIZE, 0);	//recv() will return a -1 if the socket is disconnected

		if (buffer[0] != NULL)
		{
			cout << "received: " << endl << buffer << endl << endl;
		}

		buffer[0] = '\0';
	}	

	cout << "disconnected" << endl << endl;
	closesocket(mSocket);	//close the socket

	return;


}