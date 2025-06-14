#pragma once

#include <string>
#include <tuple>
#include <ctime>
#include "SqliteDatabase.h"

using std::string;
using std::tuple;

struct UserStatistics {
    string username;
    time_t averageAnswerTime;
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
