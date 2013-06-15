#include "ClientFunct.h"


int main (void)
{

	CommClient mClient("network_config.cfg");
	mClient.Start();

	return 0;
}