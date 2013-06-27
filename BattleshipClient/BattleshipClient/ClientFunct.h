#ifndef CLIENTFUNCT_H
#define CLIENTFUNCT_H

//Libraries
#include <iostream>
#include <winsock.h>
#include <thread>
#include <string>
#include <fstream>


//Using Statements
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::thread;
using std::string;
using std::fstream;
using std::ofstream;
using std::ifstream;
//using namespace std;

#define SCK_VERSION2 0x0202
#define BUFSIZE 128

//Formatting
#define WATER_CHAR '-'
#define MISS_CHAR  'x'
#define HIT_CHAR   'O'

class Point
{
public:
	Point();
	~Point();

private:

//class members
	short int x;
	short int y;
	

};


class GameBoard
{
public:
	GameBoard(int size);
	~GameBoard();
	void Display(void);
	void Place(int xstart, int xend, int ystart, int yend, char type);
	bool Shoot(int x, int y);

private:
	
//class members	
	int mArraysize;
	char** mArray;
};


typedef struct coordinate
{
	short int x;
	short int y;

} Coordinate;


class Ship
{
public:
	Ship(short int size);
	~Ship();

private:

//class members
	Coordinate* mLocations;
	short int mSize;

};

#endif