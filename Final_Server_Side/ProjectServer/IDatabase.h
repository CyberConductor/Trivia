#pragma once
#include "winsqlite/winsqlite3.h"
#include "Question.h"
#include <vector>
#include <string>
#include <list>
#include <ctime>
#include <map>

using std::list;
using std::string;
using std::vector;
using std::map;

class IDatabase
{
public:
    virtual ~IDatabase() = default;

    virtual bool open() = 0;
    virtual bool close() = 0;

    virtual bool doesUserExist(string username) = 0;
    virtual bool doesPasswordMatch(string username, string password) = 0;
    virtual int addNewUser(string username, string password, string emailAddr) = 0;

    virtual vector<Question> getQuestion(int) = 0;
    virtual time_t getPlayerAverageAnswerTime(string) = 0;
    virtual int getNumOfAnswers(string) = 0;
    virtual int getNumOfCorrectAnswers(string) = 0;
    virtual int getNumOfPlayerGames(string) = 0;
    virtual int getPlayerScore(string) = 0;
    virtual map<string, int> getHighScores() = 0;
};