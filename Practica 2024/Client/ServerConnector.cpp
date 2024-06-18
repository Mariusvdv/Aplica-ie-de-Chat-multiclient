#include "ServerConnector.h"

SOCKET ServerConnector::sock;
ServerConnector* ServerConnector::instance = nullptr;

ServerConnector::ServerConnector() {
    std::cout << "Am pornit conexiunea catre server\n\n";

    // Crearea socket-ului pentru client
    sock= socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Eroare la crearea socket-ului.\n";
        return;
    }

    // Setarea informațiilor de adresa pentru server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Conectarea la server
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Eroare la conectarea la server.\n";
        closesocket(sock);
        return ;
    }

}
ServerConnector::~ServerConnector()
{
    std::cout << "Conexiunea la server a fost distrusa\n\n";
}

ServerConnector& ServerConnector::createServerConnector()
{
    if (instance == nullptr)
        instance = new ServerConnector();
    return *instance;
}

void ServerConnector::deleteServerConnector()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void ServerConnector::sendMessage(const std::string mesaj)
{
    send(sock, mesaj.c_str(), strlen(mesaj.c_str()), 0);
}

std::string ServerConnector::receiveMessage()
{
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
    if (bytesReceived == -1) {
        std::cerr << "Eroare la primirea mesajului de la server.\n";
    }
    else {
        std::cout << "Mesaj primit de la server: " << buffer << "\n";
    }
    std::string str(buffer);
    return str;
}
