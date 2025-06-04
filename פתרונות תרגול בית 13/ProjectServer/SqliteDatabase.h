#pragma once
#include <vector>
#include "IDatabase.h"
using std::string;

using std::vector;
using std::tuple;
class SqliteDatabase : public IDatabase
{
public:
    SqliteDatabase();
    ~SqliteDatabase();

    bool open() override;
    bool close() override;
    int doesUserExist(string username) override;
    int doesPasswordMatch(string username, string password) override;
    int addNewUser(string username, string password, string emailAddr) override;
    vector<tuple<string, int>> getTopHighScores(int count);

    virtual list<Question> getQuestion(int);
    virtual float getPlayerAverageAnswerTime(string);
    virtual int getNumOfCorrectAnswers(string);
    virtual int getNumOfPlayerGames(string);
    virtual int getPlayerScore(string);
    virtual vector<string> getHighScores();
private:
    sqlite3* _db;
};
