#pragma once

#include<iostream>
#include<string>
#include <WS2tcpip.h>
#include<vector>
#include<thread>


//#include <sys/socket.h>
#include <cerrno>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "odbc32.lib")
#include"Utilizator.h"



class ClientConnector
{
private:
    static ClientConnector* instance;
    static SOCKET listening;

    ClientConnector() ;
    ~ClientConnector() ;
    static void initializareSoketAscultare();
    static void creereSoketNou();
    static void initializareServer();
    static std::vector<Utilizator*> utilizatori;

    
public:
    static ClientConnector& createClientConnector();
    static void deleteClientConnector();


    static void printUtilizatori();

    static void sendMessage(const SOCKET client_socket, const std::string message);
    static std::string receiveMessage(SOCKET client_socket);

    static void Menu(SOCKET sock);
    static void chooseDestination(SOCKET sock);
    static void Conversation(SOCKET sock);
};

