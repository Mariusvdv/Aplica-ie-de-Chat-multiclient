#pragma once

#include<iostream>
#include<string>
//#include <WS2tcpip.h>

#include <sys/socket.h>  // pentru funcții de socket
#include <netinet/in.h>  // pentru structuri de adrese IP

#include<vector>
#include<thread>


//#include <sys/socket.h>
#include <cerrno>

//#pragma comment (lib, "ws2_32.lib")
//#pragma comment (lib, "odbc32.lib")
#include"Utilizator.h"



class ClientConnector
{
private:
    static ClientConnector* instance;
    static int listening;

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

    static void sendMessage(const int &client_socket, const std::string message);
    static std::string receiveMessage(int &client_socket);

    static void Menu(int &sock);
    static void chooseDestination(int &sock);
    static void Conversation(int &sock);
};

