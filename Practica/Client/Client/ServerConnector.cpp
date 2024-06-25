#include "ServerConnector.h"
#include"Menu.h"
#include <unistd.h>
#include <arpa/inet.h>


const char* SERVER_IP = "127.0.0.1";
const int PORT = 54000;
int ServerConnector::sock;
ServerConnector* ServerConnector::instance = nullptr;
std::string ServerConnector::nume;


ServerConnector::ServerConnector() {
    
    ServerConnector::initializareConexiuneServer();
    //std::string nume;
    std::cout << "Te rog sa-ti alegi un username pt a participa la chat: ";
    std::cin >> nume;
    std::cout << "\n\n";
    ServerConnector::sendMessage(nume);

    Menu::createMenu(nume);

   
}
ServerConnector::~ServerConnector()
{
    std::cout << "Conexiunea la server a fost distrusa\n\n";
    close(sock);
}

void ServerConnector::initializareConexiuneServer()
{
    
    // Crearea socket-ului pentru client
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Eroare la crearea socket-ului.\n\n";
        close(sock);
                std::cout << "Serverul este oprit, aplicatia se va opri in 5 secunde\n\n";
        std::cout << " 5 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Așteaptă 5 secunde
        std::cout << " 4 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 3 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 2 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 1 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      
        std::cout << "La revedere" << std::endl;
        exit(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    std::cout << "Am creat cu succes socketul " << sock << "\n\n";

    // Setarea informațiilor de adresa pentru server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Conectarea la server
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Eroare la conectarea la server.\n\n";
        close(sock);
        std::cout << "Serverul este oprit, aplicatia se va opri in 5 secunde\n\n";
        std::cout << " 5 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Așteaptă 5 secunde
        std::cout << " 4 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 3 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 2 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 1 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "La revedere" << std::endl;
        //exit;
        exit(EXIT_FAILURE);
        //return;
    }
    else
        std::cout << "Conectat cu succes la server\n\n";


    std::cout << "Am pornit conexiunea catre server\n\n";
   
    


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
        close(sock);
        std::cout << "Serverul este oprit, aplicatia se va opri in 5 secunde\n\n";
        std::cout << " 5 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Așteaptă 5 secunde
        std::cout << " 4 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 3 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 2 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << " 1 " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      
        std::cout << "La revedere" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        std::cout << "Mesaj primit de la server: " << buffer << "\n";
    }
    std::string str(buffer);
    return str;
}
