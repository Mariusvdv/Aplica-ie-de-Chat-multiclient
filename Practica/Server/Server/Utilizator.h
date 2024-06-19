#pragma once

#include<iostream>
#include<vector>
#include <cerrno>
#include <WS2tcpip.h>
#include<thread>



class Utilizator
{

private:
	std::string nume;
	void runUserThread();
	SOCKET sock;
	int nom = 0;
	std::vector<std::string> messages;
	std::thread userThread;
public:

	Utilizator( SOCKET sock1) :sock(sock1) { userThread = std::thread(&Utilizator::runUserThread, this); 
	userThread.detach();
	};

	~Utilizator(){ userThread.join(); }
	//void joinThread() { userThread.join(); }
	//void detachThread() { userThread.detach(); }
	const std::string getName()const { return nume; }
    const SOCKET getSocket() const { return sock; }
	const std::vector<std::string> getMessages() const { return messages; }
	const int getNumberOfMessages() { return nom; }
	friend std::ostream& operator<<(std::ostream& os, const Utilizator& user);

};
