#include "Menu.h"
#include "ServerConnector.h"
#include <string>

#include "Conversation.h"

Menu* Menu::instance = nullptr;

std::string Menu::sursa;

Menu::Menu(std::string surs)
{
	sursa = surs;
	system("clear");
	Conversation::deleteConversation();
	std::cout << "\t\tMeniu\n\nConversatii private:\n\n";
	//primesc toti userii din baza de date;
	//std::cout << "A\n";
	//std::cout << "B\n";

	ServerConnector::sendMessage("MENU");

	int noc = stoi(ServerConnector::receiveMessage());
	 ServerConnector::sendMessage("ack");

	for (int i = 0; i < noc; i++)
		{std::cout << ServerConnector::receiveMessage() << "\n";
		 ServerConnector::sendMessage("ack");
		}

	std::cout << "\nGrupuri:\n\n";

	noc = stoi(ServerConnector::receiveMessage());
	 ServerConnector::sendMessage("ack");

	for (int i = 0; i < noc; i++)
		{std::cout << ServerConnector::receiveMessage() << "\n";
		 ServerConnector::sendMessage("ack");
		}



	std::cout << "\nAlege cu cine vrei sa conversezi: ";
	std::string destinatie;
	//destinatie.clear();
	ServerConnector::sendMessage("VerifyDestination");
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//std::cin.clear();
	std::getline(std::cin, destinatie);
	//std::cin.clear();
	//std::cin.clear();
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	 
	//std::cin >> destinatie;
	std::cin.clear();
	std::string ack=ServerConnector::receiveMessage();
	ServerConnector::sendMessage(surs);
		ServerConnector::receiveMessage();
	// std::cout<<"Trimit1 "<<destinatie<<"\n";
	ServerConnector::sendMessage(destinatie);

	
	while (ServerConnector::receiveMessage() != "ok")
	{
		std::cout << "\nDestinatar invalid. Reincearca: \n\n";
		ServerConnector::sendMessage("VerifyDestination");
		//std::cin >> destinatie;
		ServerConnector::receiveMessage();
		//ServerConnector::sendMessage("VerifyDestination");
		std::cin.clear();
//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//std::cin.clear();
	std::getline(std::cin, destinatie);
		
		ServerConnector::sendMessage(surs);
		ServerConnector::receiveMessage();
		ServerConnector::sendMessage(destinatie);
	}
	Conversation::createConversation(sursa, destinatie);

}

Menu::~Menu()
{
	//opresc meniul
}

Menu& Menu::createMenu(std::string sursa)
{
	if (instance == nullptr)
		instance = new Menu(sursa);
	return *instance;
}

void Menu::deleteMenu()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}
