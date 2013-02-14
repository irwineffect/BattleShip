#include "ServerFunct.h"


int main (void)
{

	cout << "Hello World" << endl;

	ConnectToHost(4430, "127.0.0.1");

	WSACleanup();

	return 0;
}
