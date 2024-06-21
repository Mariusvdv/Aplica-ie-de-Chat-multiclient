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
    //char connString[] = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=172.17.0.2,1433;DATABASE=Multichat;UID=;PWD=Floridetei.02.am;";
    char connString[] = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=127.0.0.1;DATABASE=Multichat;UID=sa;PWD=Floridetei.02.am;";

    retcode = SQLDriverConnect(hdbc, NULL, (SQLCHAR*)connString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

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



int DbConnector::numaraRanduri(std::string afterFrom)
{
    SQLHANDLE hStmt;
    SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    ret = SQLFreeStmt(hStmt, SQL_CLOSE); // Eliberăm statementul anterior

    // Re-alocăm handle-ul pentru a executa o nouă interogare
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    int numarUtilizatori = 0;
    //int nrAnterior = std::stoi(ClientConector::receive_message(socket));
   // int x = 0;
    //nrAnterior += 10;
    // Executare interogare

    std::string cautare;

    //cautare = "SELECT username FROM users WHERE id > " + std::to_string(nrAnterior) + " AND id <= " + std::to_string(nrAnterior + 10);
    std::string g = "users";
    cautare = "SELECT COUNT(*) FROM " + afterFrom;
    //cautare = "SELECT username FROM users WHERE id > " + std::to_string(0) + " AND id <= " + std::to_string(10);

   // std::string cautare = "SELECT COUNT(*) FROM " + afterFrom;

//std::string myString = "text de convertit";
//const char* charStr = myString.c_str();
//SQLCHAR* sqlCharStr = reinterpret_cast<SQLCHAR*>(const_cast<char*>(charStr));


    std::wstring wide_query(cautare.begin(), cautare.end());

    std::cout<<cautare;

    std::cout<<50;
// Execută interogarea SQL
    ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        // Citire rezultat

        std::cout<<10;
       // std::string var;
        SQLINTEGER numar;
        ret = SQLFetch(hStmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            ret = SQLGetData(hStmt, 1, SQL_C_LONG, &numar, 0, NULL);
            std::cout<<1;
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
                std::cout<<2;
                int nr = static_cast<int>(numar);
                std::cout << nr << std::endl << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                return nr;
            }
            else
            {
                std::cout << 0 << std::endl << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                std::cout<<3;
                return 0;
            }
        }
    }
    std::cout << 0 << std::endl << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    std::cout<<4;
    return 0;
}

void DbConnector::coloanaInInterval(std::string whatISelect, std::string table, int inceputInterval, int sfarsitInterval, int socket)
{
    SQLHANDLE hStmt;
    SQLRETURN ret;

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        return;
    }

    std::string cautare = "SELECT "+whatISelect+" FROM (SELECT *, ROW_NUMBER() OVER(ORDER BY id) AS RowNum FROM " + table + ") AS UserWithRowNum WHERE RowNum >= " + std::to_string(inceputInterval) + " AND RowNum <= " + std::to_string(sfarsitInterval);

    
    std::wstring wide_query(cautare.begin(), cautare.end());

// Execută interogarea SQL
    ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);

    // ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
    
    std::cout<<cautare;
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    // Parcurgem fiecare rând rezultat
    while ((ret = SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR username[255];
        SQLLEN CBusername;

        ret = SQLGetData(hStmt, 1, SQL_C_CHAR, username, sizeof(username), &CBusername);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
            std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
        }

        // Afișăm numele de utilizator (username)
        std::cout << "Trimite " << username << std::endl;
        ClientConnector::sendMessage(socket, reinterpret_cast<const char*>(username));
    }

    // Verificăm dacă SQLFetch a eșuat din alt motiv decât SQL_NO_DATA
    if (ret != SQL_NO_DATA) {
        std::cerr << "SQLFetch error" << std::endl;
    }

    // Eliberăm handle-ul de statement
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    
    //exceptii sa deloghez clientul daca ceva nu merge
    }

bool DbConnector::verifyExistence(std::string table, std::string searchObject, std::string value)
{
    SQLHANDLE hStmt;
    SQLRETURN ret;

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        return false; //eroare
    }

    std::string cautare = "SELECT * FROM  " + table + " WHERE " + searchObject + " = '"+ value+"'";

    std::wstring wide_query(cautare.begin(), cautare.end());

    // Executați interogarea SQL
    ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false; //eroare
    }


    // Parcurgem fiecare rând rezultat
    if ((SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        return true;
    }
    else
        return false;
}
