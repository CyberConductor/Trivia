#pragma once

#include "SqliteDatabase.h"
#include <string>
#include <tuple>

using std::string;
using std::tuple;

struct UserStatistics {
    string username;
    float averageAnswerTime;
    int totalAnswers;
    int correctAnswers;
    int gamesPlayed;
    int totalScore;
};

class StatisticsManager
{
public:
    StatisticsManager(SqliteDatabase* db);

    bool doesUserExist(string);
    UserStatistics getUserStatistics(const string& username);
    vector<string> getHighScores();
private:
    SqliteDatabase* m_db;
};
