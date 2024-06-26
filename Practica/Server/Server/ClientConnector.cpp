//#pragma once
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
    close(listening);
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
            close(clientSocket);
            close(listening);
            exit(EXIT_FAILURE);
            //disconnectUtilizator(clientSocket);
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

void ClientConnector::disconnectUtilizator(int& socket) {
    for (auto it = utilizatori.begin(); it != utilizatori.end(); ++it) {
        if ((*it)->getSocket() == socket) {
            std::cout << "Deconectare utilizator cu socketul " << socket << "\n";
            delete *it;
            utilizatori.erase(it);
            break;
        }
    }
}

void ClientConnector::DeleteMessage( int &sock)
{
    ClientConnector::sendMessage(sock,"ack");
    std::string id=ClientConnector::receiveMessage(sock);
    ClientConnector::sendMessage(sock,"ack");
    std::string sursa=ClientConnector::receiveMessage(sock);
     ClientConnector::sendMessage(sock,"ack");
    std::string destinatie=ClientConnector::receiveMessage(sock);
     ClientConnector::sendMessage(sock,"ack");

     std::string command="DELETE FROM messages WHERE id = "+id+" AND sursa = (SELECT id FROM users WHERE username = '" +sursa+ "') AND destinatie = (SELECT id FROM users WHERE username = '"+destinatie+"');";
    DbConnector::execute(command);
     command="DELETE FROM messages WHERE id = "+id+" AND sursa = (SELECT id FROM users WHERE username = '" +destinatie+ "') AND destinatie = (SELECT id FROM users WHERE username = '"+sursa+"');";
DbConnector::execute(command);
     command="DELETE FROM group_messages WHERE id = "+id+" AND sursa = (SELECT id FROM users WHERE username = '" +sursa+ "') AND destinatie = (SELECT id FROM groups WHERE name = '"+destinatie+"');";
DbConnector::execute(command);
//     std::string command="DELETE FROM group_messages WHERE id = "+id+" AND sursa = (SELECT id FROM groups WHERE username = '" +destinatie+ "') AND destinatie = (SELECT id FROM users WHERE username = '"+sursa+"');";


}

void ClientConnector::printUtilizatori()
{
    for (const auto& user : ClientConnector::utilizatori) {
        std::cout << *user << "\n";
    }
}

void ClientConnector::sendMessage(const int &client_socket, const std::string message)
{
    std::cout<<"Trimit "<<message<<"\n";
    send(client_socket, message.c_str(), strlen(message.c_str()), 0);
}

std::string ClientConnector::receiveMessage(int &client_socket)
{
    std::cout<<"Astept";
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    // Primește cererea de la client
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived <=0) {
        std::cerr << "Eroare la primirea mesajului de la client.\n";
        std::cout << "Clientul cu socketul " << client_socket << " s-a deconectat.\n\n";
        //close(client_socket);
        disconnectUtilizator(client_socket);
        client_socket = -1;
        return "STOP";
    }
    else {
        std::cout << "Mesaj primit de la client: " << buffer << "\n";
    }
    
    std::string str(buffer);
    return str;
}

void ClientConnector::Menu(int &sock, std::string nume)
{
    //trimit numarul de clienti din baza de date catre noc pe client

    //clientul ii afiseaza si alege cu cine vrea sa converseze.
    //apoi eu primesc numele destinatiei si verfic daca exista
    //caz afirmativ, trimit "ok" 
    //caz negativ, cer altul

    int n = DbConnector::numaraRanduri("users");
    ClientConnector::sendMessage(sock, std::to_string(n));
    std::string ack=ClientConnector::receiveMessage(sock);
    if(ack!="ack")
    return;
    
    
    std::string query="SELECT username FROM (SELECT *, ROW_NUMBER() OVER(ORDER BY id) AS RowNum FROM users) AS UserWithRowNum WHERE RowNum <= "+std::to_string(n);
    DbConnector::selectColoana(query, 1, sock);
    
    std::string afterFrom=" groups g JOIN group_users gu ON g.id = gu.group_id JOIN users u ON gu.user_id = u.id WHERE u.username = '"+nume+"';";
    n = DbConnector::numaraRanduri(afterFrom);
    ClientConnector::sendMessage(sock, std::to_string(n));
    ack=ClientConnector::receiveMessage(sock);

    query=" SELECT UserWithRowNum.name FROM (SELECT *, ROW_NUMBER() OVER(ORDER BY id) AS RowNum FROM groups) AS UserWithRowNum JOIN group_users gu ON UserWithRowNum.id = gu.group_id JOIN users u ON gu.user_id = u.id WHERE RowNum <= "+std::to_string(n)+" AND u.username = '"+nume +"';";
    DbConnector::selectColoana(query, 1, sock);

}

void ClientConnector::chooseDestination(int &sock)
{
    ClientConnector::sendMessage(sock, "ack");
    std::string source = ClientConnector::receiveMessage(sock);

    ClientConnector::sendMessage(sock, "ack");
    std::string destination = ClientConnector::receiveMessage(sock);

    if (DbConnector::verifyExistence("users", "username", destination)==true||DbConnector::verifyExistenceUserInGrup("groups", "name", destination,source)==true)
    
    {
        ClientConnector::sendMessage(sock, "ok");
    }
    else
    {
        ClientConnector::sendMessage(sock, "notok");
    }
}

void ClientConnector::Conversation(int &sock)
{
    ClientConnector::sendMessage(sock,"ack");
    std::string username1=ClientConnector::receiveMessage(sock);
    ClientConnector::sendMessage(sock,"ack");
    std::string username2=ClientConnector::receiveMessage(sock);

    // ClientConnector::sendMessage(sock,"ack");
    // std::string type=ClientConnector::receiveMessage(sock);

    std::string type;

    if(DbConnector::verifyExistence("users", "username", username2)==true)
    {
        type="private";
        
    }
    else{
        if(DbConnector::verifyExistenceUserInGrup("groups", "name", username2,username1)==true)
        {
            type="group";
        }
    }
    std::cout<<"\n"<<type<<"\n\n";
    

    std::string query;
    std::string afterFrom;
    if(type =="private")
    {
    afterFrom= " messages m "
                        "JOIN users u1 ON m.sursa = u1.id "
                        "JOIN users u2 ON m.destinatie = u2.id "
                        "WHERE (u1.username = '" + username1 + "' AND u2.username = '" + username2 + "') "
                        "   OR (u1.username = '" + username2 + "' AND u2.username = '" + username1 + "');";
    }
    else{
         afterFrom= " group_messages m "
                        "JOIN users u1 ON m.sursa = u1.id "
                        "JOIN groups u2 ON m.destinatie = u2.id "
                        "WHERE ( u2.name = '" + username2 + "') "
                        "   OR ( u2.name = '" + username1 + "');";
    }
    int n = DbConnector::numaraRanduri(afterFrom);
    std::cout<<n;

    
        //std::cout<<std::endl<<n<<std::endl;
        ClientConnector::sendMessage(sock, std::to_string(n));
        std::string ack = ClientConnector::receiveMessage(sock);
    if(type=="private")
    query=

    
      "  SELECT  UserWithRowNum.id,  UserWithRowNum.mesaj, u1.username AS sursa_username "
"        FROM ( "
 "           SELECT m.id, m.mesaj, m.sursa, "
  "              ROW_NUMBER() OVER (ORDER BY m.id) AS RowNum "
   "         FROM messages m "
    "        JOIN users u1 ON m.sursa = u1.id "
     "       JOIN users u2 ON m.destinatie = u2.id "
      "      WHERE (u1.username = '" +username1+"' AND u2.username = '"+username2+"') "
       "         OR (u1.username = '"+username2+"' AND u2.username = '" +username1+"') "
        ") AS UserWithRowNum "
        "JOIN users u1 ON UserWithRowNum.sursa = u1.id "
        "WHERE RowNum <= "+std::to_string(n)+"; ";
        else{

              query=   
      "  SELECT  UserWithRowNum.id,  UserWithRowNum.mesaj, u1.username AS sursa_username "
"        FROM ( "
 "           SELECT m.id, m.mesaj, m.sursa, "
  "              ROW_NUMBER() OVER (ORDER BY m.id) AS RowNum "
   "         FROM group_messages m "
    "        JOIN users u1 ON m.sursa = u1.id "
     "       JOIN groups u2 ON m.destinatie = u2.id "
      "      WHERE ( u2.name = '"+username2+"') "
       "         OR ( u2.name = '" +username1+"') "
        ") AS UserWithRowNum "
        "JOIN users u1 ON UserWithRowNum.sursa = u1.id "
        "WHERE RowNum <= "+std::to_string(n)+"; ";
        }
   

        DbConnector::selectColoana(query, 3, sock);
    
        
    

}

void ClientConnector::ChatMessage(int &sock)
{
    ClientConnector::sendMessage(sock,"ack");
    std::string sursa=ClientConnector::receiveMessage(sock);
    ClientConnector::sendMessage(sock,"ack");
    std::string destinatia=ClientConnector::receiveMessage(sock);
    ClientConnector::sendMessage(sock,"ack");
    std::string mesaj=ClientConnector::receiveMessage(sock);

    std::string type;

    if(DbConnector::verifyExistence("users", "username", destinatia)==true)
    {
        type="private";
        
    }
    else{
        if(DbConnector::verifyExistenceUserInGrup("groups", "name", destinatia,sursa)==true)
        {
            type="group";
        }
    }
    std::cout<<"\n"<<type<<"\n\n";

    DbConnector::ChatMessage(sursa,destinatia,mesaj,type);
    ClientConnector::sendMessage(sock,"ackafterInsert");
    
}
