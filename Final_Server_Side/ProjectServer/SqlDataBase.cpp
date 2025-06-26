#include "SqliteDataBase.h"
#include <iostream>

SqliteDataBase::SqliteDataBase(const std::string& dbPath): 
    m_dbPath(dbPath), m_db(nullptr) {}

SqliteDataBase::~SqliteDataBase()
{
    close();
}

bool SqliteDataBase::open()
{
    int rc = sqlite3_open(m_dbPath.c_str(), &m_db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }
    return true;
}

void SqliteDataBase::close()
{
    if (m_db)
    {
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}

bool SqliteDataBase::execute(const std::string& query)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << (errMsg ? errMsg : "Unknown error") << std::endl;
        if (errMsg) sqlite3_free(errMsg);
        return false;
    }
    return true;
}

static int callback(void* data, int argc, char** argv, char** azColName)
{
    auto* results = static_cast<std::vector<std::unordered_map<std::string, std::string>>*>(data);
    std::unordered_map<std::string, std::string> row;
    for (int i = 0; i < argc; ++i)
    {
        row[azColName[i]] = argv[i] ? argv[i] : "";
    }
    results->push_back(row);
    return 0;
}

std::vector<std::unordered_map<std::string, std::string>> SqliteDataBase::select(const std::string& query)
{
    std::vector<std::unordered_map<std::string, std::string>> results;
    char* errMsg = nullptr;
    int rc = sqlite3_exec(m_db, query.c_str(), callback, &results, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Select failed: " << (errMsg ? errMsg : "Unknown error") << std::endl;
        if (errMsg) sqlite3_free(errMsg);
    }
    return results;
}
