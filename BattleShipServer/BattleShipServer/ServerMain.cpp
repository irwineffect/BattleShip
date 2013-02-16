#include "ServerFunct.h"


int main (void)
{

	int mPort = 3410;
	char mIpAddr[16] = "127.0.0.1";
	SOCKET mListenSocket, tempSocket;
	//SOCKET mClients[MAXCLIENTS];
	thread mClients[MAXCLIENTS];
	int numClients = 0;
	int numThreads = 0;

	WSADATA wsadata;
	SOCKADDR listen_socket_info;
	int socket_size = sizeof(listen_socket_info);
	char buffer[BUFSIZE] = "";


	cout << "Starting Server" << endl;

	if (WSAStartup(SCK_VERSION1, &wsadata))
		cout << "WSAStartup failed" << endl;
	else
		cout << "WSAStartup successful" << endl;

	if (startListening(mPort, mIpAddr, &mListenSocket))
	{
		do
		{
			tempSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);
			mClients[numThreads].swap(new thread(talk, tempSocket, &numClients));
			++numClients;
			++numThreads;
			cout << "Client Added!" << endl << numClients << " Clients Connected" << endl << endl;

		}while (numClients != 0);

		for (int i=0; i < numThreads; ++i)
		{
			mClients[i].join();
		}

	}

	cout << "Done talking!" << endl;
	//system("pause");

	WSACleanup();

	return 0;
}
