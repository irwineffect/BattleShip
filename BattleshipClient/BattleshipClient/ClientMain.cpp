#include "ClientFunct.h"


int main (void)
{

	CommClient mClient(3410, "wycoffranch.dyndns.org");
	mClient.Start();

	return 0;
}