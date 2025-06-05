#pragma once
#include <vector>
#include "IDatabase.h"

class SqliteDatabase : public IDatabase
{
public:
    SqliteDatabase();
    ~SqliteDatabase();

    bool open() override;
    bool close() override;
    int doesUserExist(string) override;
    int doesPasswordMatch(string, string) override;
    int addNewUser(string, string, string) override;

    virtual list<Question> getQuestion(int);
    virtual float getPlayerAverageAnswerTime(string);
    virtual int getNumOfAnswers(string);
    virtual int getNumOfCorrectAnswers(string);
    virtual int getNumOfPlayerGames(string);
    virtual int getPlayerScore(string);
    virtual vector<tuple<string, int>> getHighScores();
private:
    sqlite3* _db;
};
