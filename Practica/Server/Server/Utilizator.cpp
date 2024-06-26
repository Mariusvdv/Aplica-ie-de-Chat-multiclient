#include "Utilizator.h"
#include"DbConnector.h"
#include "ClientConnector.h"
std::ostream& operator<<(std::ostream& os, const Utilizator& user) {
    os << "Nume: " << user.nume << "\n";
    os << "Socket: " << user.sock << "\n";
    //os << "Nom: " << user.nom << "\n";
  /*  os << "Mesaje:\n";
    for (const auto& message : user.getMessages()) {
        os << message << "\n";
    }*/
    return os;
}

void Utilizator::runUserThread()
{
    nume = ClientConnector::receiveMessage(sock);
    
    DbConnector::log(nume);

    while (running)
    {
        std::string action = ClientConnector::receiveMessage(sock);
        if (action == "MENU")
        {
            ClientConnector::Menu(sock,nume);
        }else
        if (action == "VerifyDestination")
        {
            ClientConnector::chooseDestination(sock);
        }else
        if (action == "CONVERSATION")
        {
            ClientConnector::Conversation(sock);

        }else
        if(action=="MESAJ")
        {
            ClientConnector::ChatMessage(sock);
        }else

        if(action=="DELETE")
        {
            ClientConnector::DeleteMessage(sock);
        }else
        if(action=="ack")
        {
            continue;
        }else{std::cout<<action;}

    }
    //ClientConnector::printUtilizatori();
}
