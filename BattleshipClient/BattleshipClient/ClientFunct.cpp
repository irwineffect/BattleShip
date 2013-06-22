#include "ClientFunct.h"

/***************************************************************************************************************
****************************************************************************************************************
*	CommClient Class
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
CommClient::CommClient(string filename)
{
	WSADATA wsadata; //variable used for initializing the socket software stuff
	ifstream cfgfile_in;

	WSAStartup(SCK_VERSION2, &wsadata);	//initialize socket stuff

	cfgfile_in.open(filename);

	if (cfgfile_in.is_open() && ParseFile(cfgfile_in) )
	{
		cout << "Config File loaded successfully!" << endl;
		cfgfile_in.close();
	}
	else
	{
		ofstream cfgfile_out;
		cout << "Could not open configuration file, please input values manually" << endl << endl;
		cout << "Input hostname> ";
		cin >> mHostname;
		cout << "Input port> ";
		cin >> mPort;

		cfgfile_out.open(filename);
		cfgfile_out << "hostname:" << mHostname << endl;
		cfgfile_out << "port:" << mPort << endl;
		cfgfile_out.close();
	}



	if(mHostname[0] == '0') //if the null hostname is specified
	{
		this->socket_info.sin_addr.s_addr = inet_addr("127.0.0.1"); //use the loopback address

	}
	else //a hostname is given
	{
		struct hostent *remoteHost = NULL;
		remoteHost = gethostbyname(mHostname.c_str()); //attempt DNS lookup

		if( remoteHost == NULL)
		{
			char mIpAddr[32] = "";
			cout << "Could not resolve hostname, please enter ip address manually" << endl;
			cin >> mIpAddr;
			this->socket_info.sin_addr.s_addr = inet_addr(mIpAddr);
		}
		else
		{
			this->socket_info.sin_addr.s_addr = *(u_long*)remoteHost->h_addr_list[0];
		}
	}

	this->run = true;
	this->socket_info.sin_family = AF_INET;	//use ipv4
	this->socket_info.sin_port = htons(mPort);	//use selected port
}
CommClient::~CommClient(void)
{

	WSACleanup();	//not sure exactly what this does, but we're always supposed to call this at the end (I think it closes sockets, etc) 
}

bool CommClient::Start(void)
{
	int connect_status;

	cout << "Starting Client" << endl;	

	this->mSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);	//initialze the socket to use ipv4, tcp streaming
	if (this->mSocket == INVALID_SOCKET)
		cout << "Socket creation failed" << endl;
	else
		cout << "Socket creation successful" << endl;

	//attempt to connect to the server
	//this will fail if no server is listening
	connect_status = connect(this->mSocket, (SOCKADDR*) &socket_info, sizeof(socket_info));	

	if (connect_status == SOCKET_ERROR)
	{
		cout << "Socket connection failed" << endl;
		return false;
	}
	else
	{
		cout << "Socket connection successful" << endl;
		thread temp(&CommClient::Receiver, this, mSocket);
		mReciever_thread.swap(temp);
		return true;
	}

}
void CommClient::End(void)
{
	if (mReciever_thread.joinable())
	{
		mReciever_thread.join();
	}

	closesocket(mSocket);	//close the socket

}

bool CommClient::SendMsg(string message)
{
	int sent_count     = 0,
		message_length = 0;
	char cstr_message[BUFSIZE] = "";



	message_length = message.length();


	if (message_length >= BUFSIZE)	//if the message is too large
	{
		return false;	//fail the command
	}

	for (int i=0; i < message_length; ++i) //copy the message into a c-style string
	{
		cstr_message[i] = message[i];
	}

	cstr_message[message_length] = '\0'; //ensure the c-style string is null-terminated


	sent_count = send(mSocket, cstr_message, message_length, 0);	//send the message to the server

	if (sent_count != message_length)	//we did not send the whole message
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CommClient::Receiver(SOCKET mSocket)
{
	char buffer[BUFSIZE] = "";	//data buffer
	int connected = 1;	

	//TEST mRecv;

	while( connected != -1  && run==true)
	{
		//recv() is a blocking function, meaning program flow will halt here until
		//somebody sends something
		connected = recv(mSocket, buffer, BUFSIZE, 0);	//recv() will return a -1 if the socket is disconnected		

		if (buffer[0] != NULL)
		{
			cout << endl << "received: " << endl << buffer << endl << endl;
		}

		buffer[0] = '\0';
	}	

	cout << "disconnected" << endl << endl;
	closesocket(mSocket);	//close the socket

	return;
}


bool CommClient::ParseFile(ifstream &cfgfile)
{
	string buffer;
	string parameter;
	string value;
	int position;

	//initialize values
	mHostname = "";
	mPort = 0;

	while ( cfgfile.good() )
	{
		getline(cfgfile, buffer);
		position = buffer.find(':');
		parameter = buffer.substr( 0, position );
		value = buffer.substr( position+1, buffer.length() );

		if (parameter == "hostname")
		{
			mHostname = value;
		}
		else if (parameter == "port")
		{
			mPort = atoi( value.c_str() );
		}
	}

	if (mHostname == "" || mPort == 0)
		return false;
	else
		return true;

}

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

