#pragma once
#include "IDatabase.h"

class SqliteDatabase : public IDatabase
{
public:
    SqliteDatabase();
    ~SqliteDatabase();

    bool open() override;
    bool close() override;
    int doesUserExist(string username) override;
    int doesPasswordMatch(string username, string password) override;
    int addNewUser(string username, string password, string emailAddr) override;

private:
    sqlite3* _db;
    void createUsersTable();
};
