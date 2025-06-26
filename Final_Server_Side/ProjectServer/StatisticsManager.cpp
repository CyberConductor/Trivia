#include "StatisticsManager.h"
#include "Game.h"
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

map<string, int> StatisticsManager::getHighScores()
{
    return m_db->getHighScores();
}

int StatisticsManager::submitGameStatistics(string username, GameData data)
{
    UserStatistics stats = getUserStatistics(username);

    stats.gamesPlayed++;
    int oldTotal = stats.totalAnswers;
    stats.totalAnswers += data.correctAnswerCount + data.wrongAnswerCount;
    stats.averageAnswerTime = ((stats.averageAnswerTime * oldTotal) + data.avarageAnswerTime) / stats.totalAnswers;
    stats.correctAnswers += data.correctAnswerCount;

    // you can decide how total_score is calculated; here's a simple example:
    stats.totalScore += data.correctAnswerCount * 10; // for example, 10 points per correct answer

    string sql = "UPDATE statistics SET "
        "average_answer_time = " + std::to_string(stats.averageAnswerTime) + ", "
        "total_answers = " + std::to_string(stats.totalAnswers) + ", "
        "correct_answers = " + std::to_string(stats.correctAnswers) + ", "
        "games_played = " + std::to_string(stats.gamesPlayed) + ", "
        "total_score = " + std::to_string(stats.totalScore) +
        " WHERE username = \"" + username + "\";";

    // assuming m_database is your SqliteDatabase* and has an executeQuery method
    return m_db->execQuery(sql);
}