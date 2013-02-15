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