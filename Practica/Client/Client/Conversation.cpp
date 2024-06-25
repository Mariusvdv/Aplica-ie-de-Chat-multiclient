#include "Conversation.h"
#include "Menu.h"
#include "ServerConnector.h"
#include <iostream>
#include <string>

Conversation* Conversation::instance = nullptr;

std::string Conversation::sursa;

std::string Conversation::destinatia;



Conversation::Conversation(std::string surs, std::string destinati)
{
	sursa = surs;
	destinatia = destinati;
	system("clear");
	Menu::deleteMenu();
	std::cout << "\t\tConversatie cu"<< destinatia<<"\n\n";

	ServerConnector::sendMessage("CONVERSATION");

	std::string ack=ServerConnector::receiveMessage();
	//std::cout<<ack<<"\n";
	if(ack!="ack")
	return;

	ServerConnector::sendMessage(sursa);
	 ack=ServerConnector::receiveMessage();
	//std::cout<<ack<<"\n";
	if(ack!="ack")
	return;
	
	ServerConnector::sendMessage(destinatia);
	
	
	
	int noc = stoi(ServerConnector::receiveMessage());
	ServerConnector::sendMessage("ack");
	for (int i = 0; i < noc; i++)
		{std::cout << ServerConnector::receiveMessage() << "\n";
		 ServerConnector::sendMessage("ack");
		}
	this->runConversation(sursa, destinatia);

	//afisez ultimele zece mesaje

	// fac posibila apasarea tastei in sus pt a mai adauga un mesaj anterior si in jos pt cazul in care am mers in sus
}

Conversation::~Conversation() {

}

Conversation& Conversation::createConversation(std::string sursa, std::string destinatie)
{
	if (instance == nullptr)
	{
		
		instance = new Conversation(sursa, destinatie);
		
	}
	return *instance;
}

void Conversation::deleteConversation()
{
}

void Conversation::runConversation(std::string sursa,std::string destinatia)
{
	int nr;
	while(nr<=0||nr>3)
	{	std::cout<<"\n\nCe vrei sa faci?\n 1.Meniu\t\t 2.Refresh la mesaje\t\t 3. Mesaj nou\n\n";
	//int nr;
		std::cout<<"Introdu aici optiunea ta (doar cifra):";
		std::cin>>nr;
		//std::cin.ignore(); 
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');}
	
		switch(nr)
		{
			case 1: 
		//Menu(sursa);
			Menu::createMenu(sursa);
			break;
		
			case 2:
			deleteConversation();
			Conversation::createConversation(sursa,destinatia);
			break;

			case 3:
			std::string mesaj;
			std::cout<<"\nIntrodu noul mesaj pentru "<<destinatia<<": ";
			//std::cin>>mesaj;
			
			std::getline(std::cin, mesaj);
			std::cout<<"\n"<<mesaj<<"\n\n";

			Conversation::ChatMessage(sursa,destinatia,mesaj);

			break;
		}



		
}

void Conversation::ChatMessage(std::string sursa, std::string destinatie, std::string mesaj)
{
	ServerConnector::sendMessage("MESAJ");
	std::string ack=ServerConnector::receiveMessage();
	ServerConnector::sendMessage(sursa);
	ack=ServerConnector::receiveMessage();
	ServerConnector::sendMessage(destinatie);
	ack=ServerConnector::receiveMessage();
	ServerConnector::sendMessage(mesaj);
	ack=ServerConnector::receiveMessage();
	std::cout<<ack;
	deleteConversation();
	Conversation::createConversation(sursa,destinatia);


}
