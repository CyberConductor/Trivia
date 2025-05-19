#include "StatisticsManager.h"
#include <iostream>

using json = nlohmann::json;

StatisticsManager::StatisticsManager(SqliteDatabase* db)
    : m_db(db)
{
}

json StatisticsManager::getUserStatistics(const std::string& username)
{
    sqlite3* rawDb = m_db->getRawDB();
    ;

    std::string sql =
        "SELECT COUNT(*), AVG(score), MAX(score), MIN(score) FROM game_results WHERE username = ?;";

    sqlite3_stmt* stmt;
    json result;

    if (sqlite3_prepare_v2(rawDb, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            result["games_played"] = sqlite3_column_int(stmt, 0);
            result["average_score"] = sqlite3_column_double(stmt, 1);
            result["max_score"] = sqlite3_column_int(stmt, 2);
            result["min_score"] = sqlite3_column_int(stmt, 3);
        }
    }
    else
    {
        std::cerr << "Failed to prepare user statistics query\n";
    }

    sqlite3_finalize(stmt);
    return result;
}

json StatisticsManager::getHighScores()
{
    sqlite3* rawDb = m_db->getRawDB();

    std::string sql =
        "SELECT username, score, date FROM game_results ORDER BY score DESC LIMIT 5;";

    sqlite3_stmt* stmt;
    json results = json::array();

    if (sqlite3_prepare_v2(rawDb, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            json row;
            row["username"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            row["score"] = sqlite3_column_int(stmt, 1);
            row["date"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            results.push_back(row);
        }
    }
    else
    {
        std::cerr << "Failed to prepare high scores query\n";
    }

    sqlite3_finalize(stmt);
    return results;
}