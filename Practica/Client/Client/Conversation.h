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
	static void runConversation(std::string sursa, std::string destinatie);
	static void ChatMessage(std::string sursa, std::string destinatie,  std::string mesaj);
	static void deleteMessage(std::string sursa, std::string destinatie);
public:
	static Conversation& createConversation(std::string sursa, std::string destinatie);
	static void deleteConversation();



};

