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

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_query = converter.from_bytes(cautare);

    // Execută interogarea SQL
    ret = SQLExecDirect(hStmt, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);



    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        // Citire rezultat

       // std::string var;
        SQLINTEGER numar;
        ret = SQLFetch(hStmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            ret = SQLGetData(hStmt, 1, SQL_C_LONG, &numar, 0, NULL);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
                int nr = static_cast<int>(numar);
                std::cout << nr << std::endl << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                return nr;
            }
            else
            {
                std::cout << 0 << std::endl << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                return 0;
            }
        }
    }
    std::cout << 0 << std::endl << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    return 0;
}

void DbConnector::coloanaInInterval(std::string whatISelect, std::string table, int inceputInterval, int sfarsitInterval, SOCKET socket)
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

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_query = converter.from_bytes(cautare);

    // Executați interogarea SQL
    ret = SQLExecDirect(hStmt, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
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

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_query = converter.from_bytes(cautare);

    // Executați interogarea SQL
    ret = SQLExecDirect(hStmt, reinterpret_cast<SQLWCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
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
