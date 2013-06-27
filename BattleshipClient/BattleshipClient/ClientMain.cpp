#include "ClientFunct.h"				//Contains function specific to the Battleship Client
#include "NetFunct.h" //Contains networking functions

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