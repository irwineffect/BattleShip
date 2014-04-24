server:
	g++ Network_lib/NetFunct.cpp BattleShipServer/ServerMain.cpp -I Network_lib/ -D LINUX -std=c++0x -w -pthread -o battleship_server.exe

client:
	g++ Network_lib/NetFunct.cpp BattleShipClient/ClientMain.cpp BattleShipClient/ClientFunct.cpp -I Network_lib/ -D LINUX -std=c++0x -w -pthread -o battleship_client.exe

