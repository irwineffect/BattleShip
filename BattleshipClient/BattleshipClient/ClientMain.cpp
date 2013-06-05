#include "ClientFunct.h"


int main (void)
{

	/*CommClient mClient(3410, "wycoffranch.dyndns.org");
	mClient.Start();*/


	GameBoard mBoard(10);
	
	mBoard.Display();

	//mBoard.Place(3,3,1,5, 'B');
	mBoard.Shoot(2,8);
	mBoard.Display();

	return 0;
}