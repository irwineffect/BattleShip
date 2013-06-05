#include "ClientFunct.h"


int main (void)
{

	CommClient mClient(3410, "");
	mClient.Start();

	return 0;
}