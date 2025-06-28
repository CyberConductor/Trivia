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

//forward decleration
class Game;
typedef struct GameData GameData;

class StatisticsManager
{
public:
    StatisticsManager(SqliteDatabase* db);

    bool doesUserExist(string);
    UserStatistics getUserStatistics(const string& username);
    map<string, int> getHighScores();
    int submitGameStatistics(string, GameData);
private:
    SqliteDatabase* m_db;
};
