#include "Utilizator.h"

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
    while (true)
    {
        std::string action = ClientConnector::receiveMessage(sock);
        if (action == "MENU")
        {
            ClientConnector::Menu(sock);
        }
        if (action == "VerifyDestination")
        {
            ClientConnector::chooseDestination(sock);
        }
        if (action == "CONVERSATION")
        {
            ClientConnector::Conversation(sock);

            
        }
    }
    //ClientConnector::printUtilizatori();
}
