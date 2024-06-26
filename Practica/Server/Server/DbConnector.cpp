#include "DbConnector.h"

#include "ClientConnector.h"

//#include <thread>
#include <chrono>
#include<iostream>
#include <cstring> 



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
   // std::string g = "users";
    cautare = "SELECT COUNT(*) FROM " + afterFrom;

    std::cout<<"\n\n"<<cautare<<"\n\n";
    //cautare = "SELECT username FROM users WHERE id > " + std::to_string(0) + " AND id <= " + std::to_string(10);

   // std::string cautare = "SELECT COUNT(*) FROM " + afterFrom;

//std::string myString = "text de convertit";
//const char* charStr = myString.c_str();
//SQLCHAR* sqlCharStr = reinterpret_cast<SQLCHAR*>(const_cast<char*>(charStr));


    //std::wstring wide_query(cautare.begin(), cautare.end());

     ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);

    //std::cout<<cautare;

    //std::cout<<50;
// Execută interogarea SQL
    //ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        // Citire rezultat

        //std::cout<<10;
       // std::string var;
        SQLINTEGER numar;
        ret = SQLFetch(hStmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            ret = SQLGetData(hStmt, 1, SQL_C_LONG, &numar, 0, NULL);
            //std::cout<<1;
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
                //std::cout<<2;
                int nr = static_cast<int>(numar);
                std::cout << nr << std::endl << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                return nr;
            }
            else
            {
                std::cout << 0 << std::endl << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
               // std::cout<<3;
                return 0;
            }
        }
    }
    std::cout << 0 << std::endl << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    //std::cout<<4;
    return 0;
}

void DbConnector::selectColoana(std::string cautare, int nr,int socket)
{
    SQLHANDLE hStmt;
    SQLRETURN ret;

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    //std::string cautare = "SELECT "+whatISelect+" FROM (SELECT *, ROW_NUMBER() OVER(ORDER BY id) AS RowNum FROM " + table + ") AS UserWithRowNum WHERE RowNum <= " + std::to_string(maxx)+condition;
    ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);
    
    //std::wstring wide_query(cautare.begin(), cautare.end());

// Execută interogarea SQL
    //ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);

    // ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
    
    std::cout<<cautare;
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return;
    }

    // Parcurgem fiecare rând rezultat
    while ((ret = SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR var1[255],var2[255], var3[255];
        SQLLEN CBvar1,CBvar2,CBvar3;

       // int nr;
       if(nr==3)
        {
            ret = SQLGetData(hStmt, 1, SQL_C_CHAR, var1, sizeof(var1), &CBvar1);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
            }
            ret = SQLGetData(hStmt, 2, SQL_C_CHAR, var2, sizeof(var2), &CBvar2);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
            }
            ret = SQLGetData(hStmt, 3, SQL_C_CHAR, var3, sizeof(var3), &CBvar3);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
            }
           // var1=var1+var2;
                   char result[255 * 3]; // Ajustăm dimensiunea pentru a preveni overflow-ul
        strcpy(reinterpret_cast<char*>(result), reinterpret_cast<char*>(var1));
        strcat(reinterpret_cast<char*>(result), ": ");
        strcat(reinterpret_cast<char*>(result), reinterpret_cast<char*>(var3));
        strcat(reinterpret_cast<char*>(result), ": ");
        strcat(reinterpret_cast<char*>(result), reinterpret_cast<char*>(var2));

        // Copierea rezultatului final în var1
        strcpy(reinterpret_cast<char*>(var1), reinterpret_cast<char*>(result));
        }else
        if(nr==2)
        {
            ret = SQLGetData(hStmt, 1, SQL_C_CHAR, var1, sizeof(var1), &CBvar1);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
            }
            ret = SQLGetData(hStmt, 2, SQL_C_CHAR, var2, sizeof(var2), &CBvar2);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
            }
           // var1=var1+var2;
            strcat(reinterpret_cast<char*>(var2),": ");
            strcat(reinterpret_cast<char*>(var2), reinterpret_cast<char*>(var1));

            strcpy(reinterpret_cast<char*>(var1), reinterpret_cast<char*>(var2));
        }
        else
        {
            ret = SQLGetData(hStmt, 1, SQL_C_CHAR, var1, sizeof(var1), &CBvar1);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
                std::cerr << "SQLGetData error" << std::endl;
            continue; // Trecem la următorul rând dacă există o eroare
            }
        }
        

        // Afișăm numele de utilizator (username)
        std::cout << "Trimite " << var1 << std::endl;
        ClientConnector::sendMessage(socket, reinterpret_cast<const char*>(var1));
        std::string ack=ClientConnector::receiveMessage(socket);
        if(ack!="ack")
        {
            return;
        }
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

    // std::wstring wide_query(cautare.begin(), cautare.end());

    // // Executați interogarea SQL
    // ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
    
    ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false; //eroare
    }


    // Parcurgem fiecare rând rezultat
    if ((SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
        
    }
}

bool DbConnector::verifyExistenceUserInGrup(std::string table, std::string searchObject, std::string value, std::string user)
{
    SQLHANDLE hStmt;
    SQLRETURN ret;

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        return false; //eroare
    }

    std::string cautare = "SELECT u.username AS user_name, g.name AS group_name FROM group_users gu JOIN users u ON gu.user_id = u.id JOIN groups g ON gu.group_id = g.id WHERE u.username = '"+user+"' AND g.name = '"+value+"';";

    // std::wstring wide_query(cautare.begin(), cautare.end());

    // // Executați interogarea SQL
    // ret = SQLExecDirect(hStmt, reinterpret_cast<SQLCHAR*>(const_cast<wchar_t*>(wide_query.c_str())), SQL_NTS);
    
    ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false; //eroare
    }


    // Parcurgem fiecare rând rezultat
    if ((SQLFetch(hStmt)) == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
        
    }
}

void DbConnector::ChatMessage(std::string sursa, std::string destinatia, std::string mesaj, std::string type)
{
    SQLHANDLE hStmt;
    SQLRETURN ret;          

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        return ; //eroare
    }
   std::string cautare;
    if(type=="private")
        cautare = "INSERT INTO messages (sursa, destinatie, mesaj) "
                          "SELECT u1.id, u2.id, '" + mesaj + "' "
                          "FROM users u1, users u2 "
                          "WHERE u1.username = '" + sursa + "' AND u2.username = '" + destinatia + "'";
        else
            cautare = "INSERT INTO group_messages (sursa, destinatie, mesaj) "
                          "SELECT u1.id, u2.id, '" + mesaj + "' "
                          "FROM users u1, groups u2 "
                          "WHERE u1.username = '" + sursa + "' AND u2.name = '" + destinatia + "'";
  
    ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return; //eroare
    }


    // Parcurgem fiecare rând rezultat
    if (ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout<<"\nMesaj trimis\n\n";
        return ;
    }
    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout<<"\nMesajul nu a putut fi trimis\n\n";
        return ;
        
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
}

void DbConnector::log(std::string nume)
{
    bool b=verifyExistence("users", "username", nume);
    if(b==true)
        return;

      SQLHANDLE hStmt;
    SQLRETURN ret;

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        return ; //eroare
    }

    std::string cautare = "INSERT INTO users (username) VALUES ('" + nume + "');";
  
    ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return; //eroare
    }


    // Parcurgem fiecare rând rezultat
    if (ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout<<"\nUsername inserat cu succes\n\n";
        return ;
    }
    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout<<"\nUsername neinserat\n\n";
        return ;
        
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

}

void DbConnector::execute(std::string command)
{

      SQLHANDLE hStmt;
    SQLRETURN ret;

    // Alocați un handle de statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hStmt);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLAllocHandle error" << std::endl;
        return ; //eroare
    }

    std::string cautare = command;
  
    ret = SQLExecDirect(hStmt, (SQLCHAR*) cautare.c_str(), SQL_NTS);

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SQLExecDirect error" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return; //eroare
    }


    // Parcurgem fiecare rând rezultat
    if (ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout<<"\nexecutat\n\n";
        return ;
    }
    else
    {
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        std::cout<<"\nneexecutat\n\n";
        return ;
        
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

}

