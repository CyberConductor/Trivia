#include "StatisticsManager.h"
#include <iostream>
#include "json.hpp"
StatisticsManager::StatisticsManager(SqliteDatabase* db)
    : m_db(db){}

bool StatisticsManager::doesUserExist(string username)
{
    return m_db->doesUserExist(username);
}

UserStatistics StatisticsManager::getUserStatistics(const string& username)
{
    UserStatistics stats{};
    stats.username = username;
    stats.averageAnswerTime = m_db->getPlayerAverageAnswerTime(username);
    stats.totalAnswers = m_db->getNumOfAnswers(username);
    stats.correctAnswers = m_db->getNumOfCorrectAnswers(username);
    stats.gamesPlayed = m_db->getNumOfPlayerGames(username);
    stats.totalScore = m_db->getPlayerScore(username);

    return stats;
}

vector<string> StatisticsManager::getHighScores()
{
    vector<string> results;
    vector<tuple<string, int>> scores = m_db->getHighScores();

    for (auto& user : scores)
        results.push_back(std::get<0>(user));

    return results;
}