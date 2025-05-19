#pragma once
#include "IDatabase.h"
#include "sqlite3.h"   

class SqliteDatabase : public IDatabase
{
public:
    SqliteDatabase();
    ~SqliteDatabase();

    bool open() override;
    bool close() override;
    int doesUserExist(std::string username) override;
    int doesPasswordMatch(std::string username, std::string password) override;
    int addNewUser(std::string username, std::string password, std::string emailAddr) override;

    sqlite3* getDB() const; //getter

private:
    sqlite3* _db = nullptr;
};
