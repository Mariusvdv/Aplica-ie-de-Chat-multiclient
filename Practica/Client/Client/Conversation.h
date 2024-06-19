#pragma once
#include<iostream>
#include<cstdlib>

class Conversation
{
private:
	std::string other;
	std::string nom;//number of messages
	std::string messages[10];
public:
	Conversation();
	~Conversation();

};

