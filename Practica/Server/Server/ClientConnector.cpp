#pragma once
#include "ClientConnector.h"
#include"DbConnector.h"



const char* SERVER_IP = "127.0.0.1";
const int PORT = 54000;

ClientConnector* ClientConnector::instance = nullptr;
SOCKET ClientConnector::listening;
std::vector<Utilizator*> ClientConnector::utilizatori;

ClientConnector::ClientConnector()
{
    DbConnector::createDbConnector();
    ClientConnector::initializareServer();
    std::cout << "Serverul a fost creat\n\n";
    
    
}

ClientConnector::~ClientConnector()
{
    std::cout << "Serverul a fost distrus\n\n";
}

ClientConnector& ClientConnector::createClientConnector()
{
    if (!instance)
    {
        instance = new ClientConnector;
    }

    return *instance;

}

void ClientConnector::deleteClientConnector()
{
    if (instance!=nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}



void ClientConnector::initializareSoketAscultare()
{
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOK = WSAStartup(ver, &wsData);
    if (wsOK != 0) {
        std::cerr << "Nu s-a putut inițializa Winsock! Eroare: " << wsOK << std::endl;
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "WinSockCreat\n\n";
    // Creare socket
    listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Nu s-a putut crea socket-ul! Eroare: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }
    else
        std::cout << "Socketul de ascultare a fost creat\n\n";

    // Bind socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(listening, (sockaddr*)&hint, sizeof(hint));
    // Ascultă pentru conexiuni
    listen(listening, SOMAXCONN);
}

void ClientConnector::creereSoketNou()
{
    while (true) {
        sockaddr_in client;
        int clientSize = sizeof(client);
        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Nu s-a putut accepta conexiunea! Eroare: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
            continue;
            //WSACleanup();
            exit;
        }
        else
        {


            std::cout << "A fost acceptat socketul " << clientSocket << "\n\n";
            ClientConnector::utilizatori.push_back(new Utilizator(clientSocket));

            // ClientConnector::printUtilizatori();

             //manageUserType(clientSocket);
        }
    }
}

void ClientConnector::initializareServer()
{

    initializareSoketAscultare();
    //DbConector::createDbConector();
    //DbConector::initializareConexiuneBd();
    std::thread acceptClientsThread(creereSoketNou);
    acceptClientsThread.detach();
}

void ClientConnector::printUtilizatori()
{
    for (const auto& user : ClientConnector::utilizatori) {
        std::cout << *user << "\n";
    }
}

void ClientConnector::sendMessage(const SOCKET client_socket, const std::string message)
{
    send(client_socket, message.c_str(), strlen(message.c_str()), 0);
}

std::string ClientConnector::receiveMessage(SOCKET client_socket)
{
    char buf[4096];

    // Primește cererea de la client
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived == -1) {
        std::cerr << "Eroare la primirea mesajului de la server.\n";
        std::cout << "Clientul cu socketul " << client_socket << "s-a deconectat deconectat\n\n";
        closesocket(client_socket);
        client_socket = INVALID_SOCKET;
        //FD_CLR(current_sock, &readfds); // Eliminam socket-ul din setul de descriptori monitorizati
        //std::cout << "Client deconectat\n";
        return "STOP";
    }
    else {
        std::cout << "Mesaj primit de la server: " << buffer << "\n";
    }
    std::string str(buffer);
    return str;


}
