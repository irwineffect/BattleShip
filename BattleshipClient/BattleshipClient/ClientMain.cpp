#include "ClientFunct.h"


int main (void)
{

	CommClient mClient("network_config.cfg");

	string message;
	bool connected = true;

	mClient.Start();

	while (connected)
	{
		message.clear();
		cin >> message;
		connected = mClient.SendMsg(message);
	}




	mClient.End();
	return 0;
}