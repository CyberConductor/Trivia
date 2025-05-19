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
    sqlite3* getRawDB() const;
private:
    sqlite3* _db;
};
