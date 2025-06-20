#pragma once
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

    virtual vector<Question> getQuestion(int);
    virtual time_t getPlayerAverageAnswerTime(string);
    virtual int getNumOfAnswers(string);
    virtual int getNumOfCorrectAnswers(string);
    virtual int getNumOfPlayerGames(string);
    virtual int getPlayerScore(string);
    virtual map<string, int> getHighScores();
    int execQuery(string);
private:
    sqlite3* _db;
};
