#pragma once
#include "winsqlite/winsqlite3.h"
#include "Question.h"
#include <string>
#include <list>

using std::list;
using std::string;

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
    virtual int getNumOfCorrectAnswers(string) = 0;
    virtual int getNumOfPlayerGames(string) = 0;
    virtual int getPlayerScore(string) = 0;
    virtual vector<string> getHighScores() = 0;
};