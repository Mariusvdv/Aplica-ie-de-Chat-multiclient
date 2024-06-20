#pragma once
#include<iostream>
#include<cstdlib>

class Conversation
{
private:
	static Conversation* instance;
	std::string other;
	std::string nom;//number of messages
	std::string messages[10];

	static std::string sursa;
	static std::string destinatia;
	Conversation(std::string sursa, std::string destinatie);
	~Conversation();
public:
	static Conversation& createConversation(std::string sursa, std::string destinatie);
	static void deleteConversation();



};

