#ifndef POSTGRES_CONNECTION
#define POSTGRES_CONNECTION

#include <pqxx/pqxx>
#include <string>
#include "../DBConnection.hpp"

class PostgresConnection : public DBConnection {
public:
    PostgresConnection(std::string host, std::string port, std::string dbName, std::string password);
    ~PostgresConnection();
    void BeginTransaction() final;
    void CommitTransaction() final;
    void InsertMatch(const MatchInfo &match) final;
private:
    pqxx::connection *conn;
    pqxx::work *currentTransaction;
};


#endif

