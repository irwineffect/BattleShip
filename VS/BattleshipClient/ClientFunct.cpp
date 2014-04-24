#include "ClientFunct.h"

/***************************************************************************************************************
****************************************************************************************************************
*	Point Class
*
*
*
*
*
*
*
*
*
***************************************************************************************************************/
Point::Point()
{


	return;
}


Point::~Point()
{
}

/***************************************************************************************************************
****************************************************************************************************************
*	GameBoard Class
*
*
*
*
*
*
*
*
*
***************************************************************************************************************/
GameBoard::GameBoard(int size)
{
	int i,j;

	if (size > 26)	//limit the size of the array
		size = 26;

	mArraysize = size;

	//create the array.
	mArray = new char*[size];

	for (i=0; i<size; ++i)
		mArray[i] = new char[size];

	for (i=0; i<size; ++i)
		for(j=0; j<size; ++j)
			mArray[i][j] = WATER_CHAR;
	
	return;
}

GameBoard::~GameBoard()
{
	for (int i=0; i<mArraysize; ++i)
		delete[] mArray[i];
	delete[] mArray;

	return;
}

void GameBoard::Display(void)
{
	int i,j;

	cout << "   ";
	for (i=0; i<mArraysize; ++i)
		cout << (char)('A'+i) << ' ';
	cout << endl;

	for (i=0; i<mArraysize; ++i)
	{
		if (i>9)
			cout << i << ' ';
		else
			cout << ' ' << i << ' ';

		for (j=0; j<mArraysize; ++j)
			cout << mArray[i][j] << ' ';
		cout << endl;
	}

	return;
}


void GameBoard::Place(int xstart, int xend, int ystart, int yend, char type)
{
	int i,j;

	if (xstart >= mArraysize || xend >= mArraysize || ystart >= mArraysize || yend >= mArraysize || xstart < 0 || xend < 0 || ystart < 0 || yend < 0)
	{
		cerr << "Error: attempted to place outside array bounds" << endl;
		return;
	}

	for (i=xstart; i<=xend; ++i)
		for (j=ystart; j<=yend; ++j)
			mArray[j][i] = type;

	return;
}


bool GameBoard::Shoot(int x, int y)
{
	if (x >= mArraysize || y >= mArraysize || x < 0 || y < 0)
	{
		cerr << "Error: attempted to shoot outside array bounds" << endl;
		return false;
	}

	if (mArray[y][x] == WATER_CHAR)
	{
		mArray[y][x] = MISS_CHAR;
		return false;
	}
	else
	{
		mArray[y][x] = HIT_CHAR;
		return true;
	}

	return true;
}



/***************************************************************************************************************
****************************************************************************************************************
*	Ship Class
*
*
*
*
*
*
*
*
*
***************************************************************************************************************/
Ship::Ship(short int size)
{
	mSize = size;
	mLocations = new Coordinate[size];

	return;
}


Ship::~Ship()
{
	delete[] mLocations;
	
	return;
}

