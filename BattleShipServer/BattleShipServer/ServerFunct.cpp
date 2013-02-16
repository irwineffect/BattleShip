#include "ServerFunct.h"

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

void accepterLoop(SOCKET mListenSocket, SOCKADDR listen_socket_info, int socket_size, bool *run)
{
	SOCKET tempSocket;
	thread mClients[MAXCLIENTS];
	int numClients = 0;
	int numThreads = 0;

	u_long iMode = 1;
	ioctlsocket(mListenSocket, FIONBIO, &iMode);

	while((*run))
	{
		tempSocket = NULL;
		tempSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);

		iMode = 0;
		ioctlsocket(tempSocket, FIONBIO, &iMode);

		if (tempSocket != INVALID_SOCKET)
		{
			thread temp(talk, tempSocket, &numClients);
			mClients[numThreads].swap(temp);
			++numClients;
			++numThreads;
			cout << "Client Added!" << endl << numClients << " Clients Connected" << endl << endl;
		}
	}



	for (int i=0; i < numThreads; ++i)
	{
		mClients[i].join();
	}

}

void exitPrompt(bool* run)
{
	string buffer;
	do
	{
		cout << "Type \"exit\" to stop server" << endl;

		cin >> buffer;

		if (buffer == "exit")
			*run = false;

	}while ((*run));

	return;
}