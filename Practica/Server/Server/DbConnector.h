#pragma once
#include<iostream>

#include "ClientConnector.h"
//#include <Windows.h>
#include <sqltypes.h>

//#include <tchar.h>
#include <sql.h>
#include <sqlext.h>
#include <codecvt>

#pragma comment (lib, "libobdc.so")

class DbConnector
{

private:
	//SQLHENV henv;
	static SQLHDBC hdbc;
	//SQLRETURN retcode;

	static DbConnector* instance;
	static void initializareConexiuneBd();

	
	DbConnector() {
		initializareConexiuneBd();
		std::cout<<"Conexiunea la baza de date a pornit\n\n"; };
	~DbConnector() {std::cout<< "Conexiunea la baza de date a fost oprita\n\n"; };

public:

	static DbConnector& createDbConnector();
	static void deleteDbConnector();
	static int numaraRanduri(std::string afterFrom);
	static void selectColoana( std::string cautare,  int nrDeColoane, int socket);
	static bool verifyExistence(std::string table, std::string searchObject, std::string value);
	static bool verifyExistenceUserInGrup(std::string table, std::string searchObject, std::string value,std::string user);
	static void ChatMessage(std::string sursa, std::string destinatia, std::string mesaj, std::string type);
	static void log(std::string nume);
	static void execute(std::string command);
	
};