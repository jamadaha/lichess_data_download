#ifndef DB_CONNECTION
#define DB_CONNECTION

#include <string>
#include "parsing/types/MatchInfo.hpp"

class DBConnection {
public:
    DBConnection(std::string host, std::string port, std::string dbName, std::string password) :
        host(host), port(port), dbName(dbName), password(password) {};
    virtual void BeginTransaction() = 0;
    virtual void CommitTransaction() = 0;
    virtual void InsertMatch(const MatchInfo &match) = 0;
protected:
    std::string host;
    std::string port;
    std::string dbName;
    std::string password;
};

#endif

