#pragma once

#include "SqliteDatabase.h"
#include <string>

using std::string;

class StatisticsManager
{
public:
    StatisticsManager(SqliteDatabase* db);

    vector<string> getUserStatistics(const string& username);
    vector<string> getHighScores();
private:
    SqliteDatabase* m_db;
};
