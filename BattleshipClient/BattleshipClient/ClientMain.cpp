#include "ClientFunct.h"


int main (void)
{

	int mPort = 3410;	//port to connect on
	char mIpAddr[32] = "76.178.149.238";	//ip address to connect to (defaults
	char buffer[BUF_SIZE] = "";	//data buffer
	struct hostent *remoteHost;
	
	cout << "Starting Client" << endl;
	
	WSADATA wsadata; //variable used for initializing the socket software stuff
	
	WSAStartup(SCK_VERSION2, &wsadata);	//initialize socket stuff

	remoteHost = gethostbyname("irwineffect.dyndns.org");
	if( remoteHost == NULL)
	{
		return 1;
	}
	else
	{

	}

	//cout << "enter host ip -> ";	//enter ip address to connect to
	//cin >> mIpAddr;
	
	SOCKADDR_IN socket_info; //variable holding information for setting up the socket
	socket_info.sin_family = AF_INET;	//use ipv4
	socket_info.sin_port = htons(mPort);	//use selected port
	//socket_info.sin_addr.s_addr = inet_addr(mIpAddr);	//use selected address
	socket_info.sin_addr.s_addr = *(u_long*)remoteHost->h_addr_list[0]; //lookup ip address
	

	

	SOCKET mSocket;	//declaration of the socket
	
	mSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);	//initialze the socket to use ipv4, tcp streaming
	if (mSocket == INVALID_SOCKET)
		cout << "Socket creation failed" << endl;
	else
		cout << "Socket creation successful" << endl;
	
	int connect_status;
	
	//attempt to connect to the server
	//this will fail if no server is listening
	connect_status = connect(mSocket, (SOCKADDR*) &socket_info, sizeof(socket_info));	
	
	if (connect_status == SOCKET_ERROR)
		cout << "Socket connection failed" << endl;
	else
	{
		cout << "Socket connection successful" << endl;
		thread temp(Receiver, mSocket);
		int connected = 1;
		cout << endl << endl;
		while (1)
		{
			cin.getline(buffer, BUF_SIZE, '\n');	//read data from the user
			connected =	send(mSocket, buffer, BUF_SIZE, 0);	//send the data to the server
			cout << "Sent: " << endl << buffer << endl;

			buffer[0] = '\0';
		
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


	return 0;
}