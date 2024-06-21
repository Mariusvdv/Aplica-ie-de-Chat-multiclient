#pragma once
#include "ClientConnector.h"
#include"DbConnector.h"

#include <cstring> // Pentru memset
#include <string.h>
#include <unistd.h>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 54000;

ClientConnector* ClientConnector::instance = nullptr;
int ClientConnector::listening;
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
    
    // Creare socket
    listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        perror("Nu s-a putut crea socket-ul");
        return;
    }
    else
        std::cout << "Socketul de ascultare a fost creat\n\n";

    // Configurare adresa de legătură (bind)
    sockaddr_in hint;
    memset(&hint, 0, sizeof(hint));
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) {
        perror("Eroare la legarea socket-ului");
        close(listening); // închide socket-ul în caz de eroare
        return;
    }

    // Ascultă pentru conexiuni
    if (listen(listening, SOMAXCONN) == -1) {
        perror("Eroare la ascultarea pe socket");
        close(listening); // închide socket-ul în caz de eroare
        return;
    }

    std::cout << "Așteptăm conexiuni pe portul " << PORT << "...\n";
}

void ClientConnector::creereSoketNou()
{
    while (true) {
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

        if (clientSocket == -1) {
            perror("Eroare la acceptarea conexiunii");
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

void ClientConnector::sendMessage(const int client_socket, const std::string message)
{
    send(client_socket, message.c_str(), strlen(message.c_str()), 0);
}

std::string ClientConnector::receiveMessage(int client_socket)
{
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    // Primește cererea de la client
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived == -1) {
        std::cerr << "Eroare la primirea mesajului de la client.\n";
        std::cout << "Clientul cu socketul " << client_socket << " s-a deconectat.\n\n";
        close(client_socket);
        return "STOP";
    }
    else if (bytesReceived == 0) {
        std::cout << "Clientul cu socketul " << client_socket << " s-a deconectat.\n\n";
        close(client_socket);
        return "STOP";
    }
    else {
        std::cout << "Mesaj primit de la client: " << buffer << "\n";
    }
    
    std::string str(buffer);
    return str;
}

void ClientConnector::Menu(int sock)
{
    //trimit numarul de clienti din baza de date catre noc pe client

    //clientul ii afiseaza si alege cu cine vrea sa converseze.
    //apoi eu primesc numele destinatiei si verfic daca exista
    //caz afirmativ, trimit "ok" 
    //caz negativ, cer altul

    int n = DbConnector::numaraRanduri("users");
    if(n>0)
    {
        ClientConnector::sendMessage(sock, std::to_string(n));
        DbConnector::coloanaInInterval("username", "users", 1, 4, sock);
    }

}

void ClientConnector::chooseDestination(int sock)
{
    std::string destination = ClientConnector::receiveMessage(sock);
    if (DbConnector::verifyExistence("users", "username", destination)==true)
    {
        ClientConnector::sendMessage(sock, "ok");
    }
    else
    {
        ClientConnector::sendMessage(sock, "notok");
    }
}

void ClientConnector::Conversation(int sock)
{


}
