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
	static void coloanaInInterval(std::string whatISelect, std::string table, int inceputInterval, int sfarsitInterval, int socket);
	static bool verifyExistence(std::string table, std::string searchObject, std::string value);
	
};