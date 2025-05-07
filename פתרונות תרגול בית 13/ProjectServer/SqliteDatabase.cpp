#include "SqliteDatabase.h"
#include <winsqlite/winsqlite3.h>
#include <iostream>

using std::string;

SqliteDatabase::SqliteDatabase() {
    this->_db = nullptr;
}

SqliteDatabase::~SqliteDatabase()
{
    this->close();
    delete this->_db;
}

bool SqliteDatabase::open() {
    int res = sqlite3_open("users.db", &this->_db);
    if (res != SQLITE_OK) {
        std::cerr << "cannot open database" << std::endl;
        return false;
    }

    string createTableQuery =
        "CREATE TABLE IF NOT EXISTS users ("
        "username TEXT PRIMARY KEY, "
        "password TEXT, "
        "email TEXT);";

    res = sqlite3_exec(this->_db, createTableQuery.c_str(), nullptr, nullptr, nullptr);
    return res == SQLITE_OK;
}

bool SqliteDatabase::close() {
    if (this->_db) {
        sqlite3_close(this->_db);
        this->_db = nullptr;
        return true;
    }
    return false;
}

int SqliteDatabase::doesUserExist(string username) {
    int found = 0;
    string query = "SELECT 1 FROM users WHERE username = '" + username + "' LIMIT 1;";

    sqlite3_exec(this->_db, query.c_str(),
        [](void* data, int, char**, char**) -> int {
            *(int*)data = 1;
            return 0;
        }, &found, nullptr);

    return found;
}

int SqliteDatabase::doesPasswordMatch(string username, string password) {
    int match = 0;
    string query = "SELECT 1 FROM users WHERE username = '" + username +
        "' AND password = '" + password + "' LIMIT 1;";

    sqlite3_exec(this->_db, query.c_str(),
        [](void* data, int, char**, char**) -> int {
            *(int*)data = 1;
            return 0;
        }, &match, nullptr);

    return match;
}

int SqliteDatabase::addNewUser(string username, string password, string emailAddr) 
{
    string query = "INSERT INTO users (username, password, email) VALUES ('" +
        username + "', '" + password + "', '" + emailAddr + "');";

    int res = sqlite3_exec(this->_db, query.c_str(), nullptr, nullptr, nullptr);
    return res;
}
