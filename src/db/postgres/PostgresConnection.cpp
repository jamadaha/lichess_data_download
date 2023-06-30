#include "PostgresConnection.hpp"
#include <iostream>
#include "../SQLInsertBuiling.hpp"

PostgresConnection::PostgresConnection(std::string host, std::string port, std::string dbName, std::string password) : 
    DBConnection(host, port, dbName, password) {
    std::string connectionString = "host=" + host + " port=" + port + " dbname=" + dbName + " password=" + password;
    conn = new pqxx::connection(connectionString);
    std::cout << "Connected to " << conn->dbname() << std::endl;
}

PostgresConnection::~PostgresConnection() {
    conn->close();
    delete(conn);
}

void PostgresConnection::BeginTransaction() {
    currentTransaction = new pqxx::work(*conn);
}

void PostgresConnection::CommitTransaction() {
    currentTransaction->commit();
    delete(currentTransaction);
}

void PostgresConnection::InsertMatch(const MatchInfo &match) {
    std::string statement = SQLInsertBuilding::GenerateMatchInsert(match, "games");
    currentTransaction->exec0(statement);
}

