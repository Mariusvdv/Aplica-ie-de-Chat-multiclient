#include "Conversation.h"
#include "Menu.h"

Conversation* Conversation::instance = nullptr;

std::string Conversation::sursa;

std::string Conversation::destinatia;



Conversation::Conversation(std::string surs, std::string destinati)
{
	sursa = surs;
	destinatia = destinati;
	system("cls");
	Menu::deleteMenu();
	std::cout << "\t\tConversatie\n\n";
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
