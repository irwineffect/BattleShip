#include "ServerFunct.h"


int main (void)
{

	int mPort = 3410;
	char mIpAddr[16] = "127.0.0.1";
	SOCKET mListenSocket, mTalkSocket;
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
		mTalkSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);
		cout << "client accepted" << endl;

		cout << endl << endl;
		while (mTalkSocket)
		{
			recv(mTalkSocket, buffer, BUFSIZE, 0);
			cout << "received: " << endl << buffer << endl;
			send(mTalkSocket, buffer, BUFSIZE, 0);
			cout << "echoed!" << endl << endl;
		}
	}
	
	cout << "disconnected" << endl;
	
	cout << endl;
	system("pause");
	
	closesocket(mListenSocket);
	WSACleanup();

	return 0;
}
