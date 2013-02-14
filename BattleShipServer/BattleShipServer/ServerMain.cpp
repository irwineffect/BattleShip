#include "ServerFunct.h"


int main (void)
{

	int mPort = 3410;
	char mIpAddr[16] = "127.0.0.1";
	SOCKET mListenSocket, mTalkSocket;
	WSADATA wsadata;
	SOCKADDR_IN socket_info;
	SOCKADDR listen_socket_info;
	int socket_size = sizeof(listen_socket_info);
	char* message = "Hello";
	int number = 42;
	
	
	cout << "Starting Server" << endl;
	
	
	if (WSAStartup(SCK_VERSION1, &wsadata))
		cout << "WSAStartup failed" << endl;
	else
		cout << "WSAStartup successful" << endl;
	
	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(mPort);
	socket_info.sin_addr.s_addr = inet_addr(mIpAddr);

	
	mListenSocket = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mListenSocket == INVALID_SOCKET)
		cout << "Socket creation failed" << endl;
	else
		cout << "Socket creation successful" << endl;
	
	int bind_status;
	
	bind_status = bind(mListenSocket, (LPSOCKADDR) &socket_info, sizeof(socket_info));
	
	if (bind_status == SOCKET_ERROR)
		cout << "Socket binding failed" << endl;
	else
	{
		cout << "Socket binding successful" << endl;
		cout << "Starting to listen...";
		listen(mListenSocket, SOMAXCONN);
		cout << "listening..." << endl;
		mTalkSocket = accept(mListenSocket, (SOCKADDR*) &listen_socket_info, &socket_size);
		cout << "client accepted" << endl;
		send(mTalkSocket, (char*) &number, sizeof(int), 0);
		cout << "message sent!" << endl;
	}
	
	
	
	cout << endl;
	system("pause");
	
	closesocket(mListenSocket);
	WSACleanup();

	return 0;
}
