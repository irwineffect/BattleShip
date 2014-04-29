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
		
		if (message == "/exit")
		{
			cout << "Exiting!" << endl;
			connected = false;
		}
	}

	cout << "ending...";
	mClient.End();

	cout << "ended" << endl;

	return 0;
}
