#include "SqliteDatabase.h"
#include <iostream>
#include <string>
#include <winsqlite/winsqlite3.h>

SqliteDatabase::SqliteDatabase()
{
    this->_db = nullptr;
}

SqliteDatabase::~SqliteDatabase()
{
    this->close();
}

bool SqliteDatabase::open()
{
    int res = sqlite3_open("TriviaDB.sqlite", &this->_db);
    if (res != SQLITE_OK)
    {
        std::cerr << "failed to open db: " << sqlite3_errmsg(this->_db) << std::endl;
        return false;
    }

    string query = "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY, "
        "password TEXT NOT NULL, "
        "email TEXT NOT NULL);";
    char* errMsg = nullptr;
    res = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, &errMsg);
    if (res != SQLITE_OK)
    {
        std::cerr << "failed to create table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool SqliteDatabase::close()
{
    if (this->_db != nullptr)
    {
        sqlite3_close(this->_db);
        this->_db = nullptr;
    }
    return true;
}

int SqliteDatabase::doesUserExist(string username)
{
    string query = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr);
    if (res != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return count > 0;
}

int SqliteDatabase::doesPasswordMatch(string username, string password)
{
    string query = "SELECT password FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr);
    if (res != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    string dbPassword;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

    sqlite3_finalize(stmt);
    return dbPassword == password;
}

int SqliteDatabase::addNewUser(string username, string password, string emailAddr)
{
    string query = "INSERT INTO users (username, password, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int res = sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr);
    if (res != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, emailAddr.c_str(), -1, SQLITE_STATIC);

    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return res == SQLITE_DONE;
}
