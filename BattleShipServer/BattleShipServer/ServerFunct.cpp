#include "ServerFunct.h"

bool ConnectToHost( int PortNo, char* IPaddr)
{
	WSAData wsadata;

	if (WSAStartup(MAKEWORD(2,0), &wsadata) != 0)
	{
		cout << "WSA Startup Failed" << endl;
		return false;
	}
	else
		cout << "WSA Startup Successful" << endl;

	SOCKADDR_IN target;

	target.sin_family = AF_INET;
	target.sin_port = htons(PortNo);
	target.sin_addr.s_addr = inet_addr (IPaddr);

	SOCKET mySocket;

	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mySocket == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << endl;
		return false;
	}
	else
		cout << "Socket creation successful" << endl;

	if (connect(mySocket, (SOCKADDR*) &target, sizeof(target)) == SOCKET_ERROR)
		{
			cout << "Socket connection failed" << endl;
			return false;
		}
	else
		cout << "Socket connection sucessful" << endl;

	return true;
}



bool startListening(int PortNo, char* IPaddr, SOCKET *listener)
{

	SOCKADDR_IN socket_info;

	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(PortNo);
	socket_info.sin_addr.s_addr = inet_addr(IPaddr);

	
	*listener = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*listener == INVALID_SOCKET)
	{
		cout << "Socket creation failed" << endl;
		return false;
	}
	else
		cout << "Socket creation successful" << endl;
	
	int bind_status;
	
	bind_status = bind(*listener, (LPSOCKADDR) &socket_info, sizeof(socket_info));
	
	if (bind_status == SOCKET_ERROR)
	{
		cout << "Socket binding failed" << endl;
		return false;
	}
	else
	{
		cout << "Socket binding successful" << endl;
		cout << "Starting to listen...";
		listen(*listener, SOMAXCONN);
		cout << "listening..." << endl;
	}


	return true;
}




void CloseConnection(void)
{
	/*if (mySocket)
		closesocket(mySocket);*/
}

//void acceptclient(SOCKET *mClients, int *numClients)
//{
//	SOCKET tempSocket;
//
//
//	tempSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);
//	if (tempSocket != INVALID_SOCKET)
//	{
//		mClients[numClients++] = tempSocket;
//		cout << "client accepted" << endl;
//	}
//}



void talk(SOCKET mSocket, int* numClients)
{
	char buffer[BUFSIZE] = "";
	int connected = 1;

	while( connected != -1 )
	{
		connected = recv(mSocket, buffer, BUFSIZE, 0);

		if (buffer[0] != NULL)
		{
			cout << "received: " << endl << buffer << endl;
			strcat(buffer, " -from server");
			send(mSocket, buffer, BUFSIZE, 0);
			cout << "echoed!" << endl << endl;
		}

		buffer[0] = '\0';
	}

	cout << "client disconnected" << endl << endl;
	closesocket(mSocket);
	--(*numClients);

	return;
}
