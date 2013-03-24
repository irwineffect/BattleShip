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