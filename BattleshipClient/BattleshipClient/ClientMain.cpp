#include "ClientFunct.h"


int main (void)
{

	int mPort = 3410;
	char mIpAddr[16] = "127.0.0.1";
	char buffer[BUF_SIZE] = "";
	
	cout << "Starting Client" << endl;
	
	WSADATA wsadata;
	
	WSAStartup(SCK_VERSION2, &wsadata);
	
	SOCKADDR_IN socket_info;
	
	socket_info.sin_family = AF_INET;
	socket_info.sin_port = htons(mPort);
	socket_info.sin_addr.s_addr = inet_addr(mIpAddr);

	
	SOCKET mSocket;
	
	mSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
		cout << "Socket creation failed" << endl;
	else
		cout << "Socket creation successful" << endl;
	
	int connect_status;
	
	connect_status = connect(mSocket, (SOCKADDR*) &socket_info, sizeof(socket_info));
	
	if (connect_status == SOCKET_ERROR)
		cout << "Socket connection failed" << endl;
	else
	{
		cout << "Socket connection successful" << endl;
		


		int connected = 1;
		cout << endl << endl;
		while (1)
		{
			cin >> buffer;
			send(mSocket, buffer, BUF_SIZE, 0);
			cout << "Sent: " << endl << buffer << endl;

			buffer[0] = NULL;
			connected = recv(mSocket, buffer, BUF_SIZE, 0);
			cout << "Received:" << endl << buffer << endl << endl;

			if (connected == -1)
				break;
		}
		
	}
	
	cout << endl;
//	system("pause");

	cout << "Disconnected" << endl;
	
	closesocket(mSocket);
	WSACleanup();


	return 0;
}