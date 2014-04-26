
##flags
FLAGS=-D LINUX -w -std=c++0x -pthread

#build everything
all: server client

#make the server
server: battleship_server.exe

#make the client
client: battleship_client.exe

#create the build directory
Build:
	mkdir Build/

#build object file for networking library
Build/NetFunct.o: Build/ Network_lib/NetFunct.cpp Network_lib/NetFunct.h
	g++ -c Network_lib/NetFunct.cpp $(FLAGS) -o Build/NetFunct.o

#create the server executable
battleship_server.exe: Build/ Build/NetFunct.o BattleShipServer/ServerMain.cpp
	g++ BattleShipServer/ServerMain.cpp Build/NetFunct.o -I Network_lib/ $(FLAGS) -o battleship_server.exe

#create the client executable
battleship_client.exe: Build/ Build/NetFunct.o BattleShipClient/ClientMain.cpp BattleShipClient/ClientFunct.cpp BattleShipClient/ClientFunct.h
	g++ BattleShipClient/ClientMain.cpp BattleShipClient/ClientFunct.cpp Build/NetFunct.o -I Network_lib/ $(FLAGS) -o battleship_client.exe

#clean everything up!
clean:
	rm -rf Build/ *.exe
