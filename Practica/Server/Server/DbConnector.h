#pragma once
#include<iostream>
#include "ClientConnector.h"
#include <Windows.h>
#include <sqltypes.h>

#include <tchar.h>
#include <sql.h>
#include <sqlext.h>
#include <codecvt>

#pragma comment (lib, "odbc32.lib")

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
	
};