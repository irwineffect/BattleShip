server:
	g++ Network_lib/NetFunct.cpp BattleShipServer/BattleShipServer/ServerMain.cpp -I Network_lib/ -D LINUX -std=c++0x -w -pthread -o server.exe
