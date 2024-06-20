#pragma once

#include <iostream>
#include <winsock2.h>

#include <cstring>
#include <cstdlib>

#include <thread> // pentru std::this_thread::sleep_for
#include <chrono>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "odbc32.lib")



#include <ws2tcpip.h>




class ServerConnector
{
private:
	static SOCKET sock;
	static ServerConnector* instance;
	static std::string nume;
	ServerConnector();
	~ServerConnector();

	static void initializareConexiuneServer();
public:
	static ServerConnector& createServerConnector();
	static void deleteServerConnector();
	static void sendMessage(const std::string mesaj);
	static std::string receiveMessage();
};

