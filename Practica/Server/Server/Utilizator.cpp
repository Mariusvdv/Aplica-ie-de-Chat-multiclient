#include "Utilizator.h"

#include "ClientConnector.h"
std::ostream& operator<<(std::ostream& os, const Utilizator& user) {
    os << "Nume: " << user.nume << "\n";
    os << "Socket: " << user.sock << "\n";
    os << "Nom: " << user.nom << "\n";
    os << "Mesaje:\n";
    for (const auto& message : user.getMessages()) {
        os << message << "\n";
    }
    return os;
}

void Utilizator::runUserThread()
{
    nume = ClientConnector::receiveMessage(sock);

    ClientConnector::printUtilizatori();
}
