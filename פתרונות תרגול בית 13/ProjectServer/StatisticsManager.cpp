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