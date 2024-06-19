#include "DbConnector.h"

#include "ClientConnector.h"

//#include <thread>
#include <chrono>



SQLHDBC DbConnector::hdbc;
DbConnector* DbConnector::instance = nullptr;

void DbConnector::initializareConexiuneBd()
{
    // Conectare la baza de date
    SQLHENV henv;
    // SQLHDBC hdbc;
    SQLRETURN retcode;
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    // Definirea șirului de conexiune folosind TEXT()
    TCHAR connString[] = TEXT("DRIVER={SQL Server};SERVER=DESKTOP-K4RIHE6;DATABASE=Multichat;UID=;PWD=;");
    retcode = SQLDriverConnect(hdbc, NULL, connString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    // Verificare eroare la conectare
    if (!SQL_SUCCEEDED(retcode)) {
        std::cerr << "Eroare la conectarea la baza de date!\n\n";
        std::cout << "Vom opri serverul pentru a verifica problema\n\n";
        exit(EXIT_FAILURE);
    }
    else
        std::cout << "Conectat la baza de date\n\n";
}

DbConnector& DbConnector::createDbConnector()
{
    if (instance == nullptr)
    {
        instance = new DbConnector;
    }
    return *instance;
}

void DbConnector::deleteDbConnector()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}
