#pragma once

#include <iostream>
#include <winsock2.h>

#include <cstring>
#include <cstdlib>


#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "odbc32.lib")


#include <winsock2.h>
#include <ws2tcpip.h>


const char* SERVER_IP = "127.0.0.1";
const int PORT = 54000;

class ServerConnector
{
private:
	static SOCKET sock;
	static ServerConnector* instance;
	ServerConnector();
	~ServerConnector();
public:
	static ServerConnector& createServerConnector();
	static void deleteServerConnector();
	static void sendMessage(const std::string mesaj);
	static std::string receiveMessage();
};

