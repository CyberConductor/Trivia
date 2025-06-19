#pragma once
#include "winsqlite/winsqlite3.h"
#include "Question.h"
#include <vector>
#include <string>
#include <list>
#include <tuple>

using std::list;
using std::string;
using std::vector;
using std::tuple;

class IDatabase
{
public:
    virtual ~IDatabase() = default;

    virtual bool open() = 0;
    virtual bool close() = 0;

    virtual int doesUserExist(string username) = 0;
    virtual int doesPasswordMatch(string username, string password) = 0;
    virtual int addNewUser(string username, string password, string emailAddr) = 0;

    virtual list<Question> getQuestion(int) = 0;
    virtual float getPlayerAverageAnswerTime(string) = 0;
    virtual int getNumOfAnswers(string) = 0;
    virtual int getNumOfCorrectAnswers(string) = 0;
    virtual int getNumOfPlayerGames(string) = 0;
    virtual int getPlayerScore(string) = 0;
    virtual vector<tuple<string, int>> getHighScores() = 0;
};