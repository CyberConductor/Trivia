#pragma once

#include "SqliteDatabase.h"
#include "json.hpp"
#include <string>

class StatisticsManager
{
public:
    StatisticsManager(SqliteDatabase* db);

    nlohmann::json getUserStatistics(const std::string& username);

    //return top 5 high scores in JSON
    nlohmann::json getHighScores();

private:
    SqliteDatabase* m_db;
};
