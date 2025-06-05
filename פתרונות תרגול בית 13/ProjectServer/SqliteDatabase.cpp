#include "SqliteDatabase.h"
#include <winsqlite/winsqlite3.h>
#include <iostream>

using std::string;

SqliteDatabase::SqliteDatabase() { this->_db = nullptr; }

SqliteDatabase::~SqliteDatabase()
{
    this->close();
    delete this->_db;
}

/*
return value: if there's error->false, if not->true.
*/
bool SqliteDatabase::open() 
{
    int res = sqlite3_open("trivia.db", &this->_db);
    if (res != SQLITE_OK) 
    {
        std::cerr << "cannot open trivia database" << std::endl;
        return false;
    }

    string createUsersQuery =
        "CREATE TABLE IF NOT EXISTS users("
        "username TEXT PRIMARY KEY, "
        "password TEXT, "
        "email TEXT), "
        "score";

    res = sqlite3_exec(this->_db, createUsersQuery.c_str(), nullptr, nullptr, nullptr);
    if (res != SQLITE_OK)
        return false;

    string createQuestionsQuery =
        "CREATE TABLE IF NOT EXISTS questions("
        "question TEXT NOT NULL,"
        "correct_answer TEXT NOT NULL,"
        "incorrect_answer_1 TEXT NOT NULL,"
        "incorrect_answer_2 TEXT NOT NULL,"
        "incorrect_answer_3 TEXT NOT NULL);";

    res = sqlite3_exec(this->_db, createQuestionsQuery.c_str(), nullptr, nullptr, nullptr);
    if (res != SQLITE_OK)
        return false;

    string createStatsQuery =
        "CREATE TABLE IF NOT EXISTS statistics("
        "username TEXT PRIMARY KEY,"
        "average_answer_time REAL NOT NULL,"
        "total_answers INTEGER NOT NULL,"
        "correct_answers INTEGER NOT NULL,"
        "games_played INTEGER NOT NULL,"
        "total_score INTEGER NOT NULL"
        "FOREIGN KEY(username) REFERENCES users(username));";

    res = sqlite3_exec(this->_db, createStatsQuery.c_str(), nullptr, nullptr, nullptr);
    return res == SQLITE_OK;
}

/*
return value: if there's a database to delete->true, if not->false.
*/
bool SqliteDatabase::close() 
{
    if (this->_db) {
        sqlite3_close(this->_db);
        this->_db = nullptr;
        return true;
    }
    return false;
}

/*
return value: if found->1, if not->0.
*/
int SqliteDatabase::doesUserExist(string username)
{
    int found = 0;
    const char* query = "SELECT 1 FROM users WHERE username = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(this->_db, query, -1, &stmt, nullptr) != SQLITE_OK)
        return 0; // prepare failed

    // bind the username safely
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    // check if a row was returned
    if (sqlite3_step(stmt) == SQLITE_ROW)
        found = 1;

    sqlite3_finalize(stmt);
    return found;
}

/*
return value: if match->1, if not->0.
*/
int SqliteDatabase::doesPasswordMatch(string username, string password)
{
    int match = 0;
    const char* query = "SELECT 1 FROM users WHERE username = ? AND password = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(this->_db, query, -1, &stmt, nullptr) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
        match = 1;

    sqlite3_finalize(stmt);
    return match;
}

int SqliteDatabase::addNewUser(string username, string password, string emailAddr)
{
    const char* query = "INSERT INTO users (username, password, email) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(this->_db, query, -1, &stmt, nullptr) != SQLITE_OK)
        return SQLITE_ERROR;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, emailAddr.c_str(), -1, SQLITE_STATIC);

    int res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return res == SQLITE_DONE ? SQLITE_OK : SQLITE_ERROR;
}

list<Question> SqliteDatabase::getQuestion(int num)
{
    list<Question> questions;
    string query = "SELECT question, correct_answer, incorrect_answer_1, incorrect_answer_2, incorrect_answer_3 FROM questions LIMIT " + std::to_string(num) + ";";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "failed to prepare statement" << std::endl;
        return questions;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string questionText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string correctAnswer = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string incorrect1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string incorrect2 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string incorrect3 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

        vector<string> answers = { correctAnswer, incorrect1, incorrect2, incorrect3 };

        questions.push_back(Question(questionText, correctAnswer, answers));
    }

    sqlite3_finalize(stmt);
    return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(string username)
{
    string query = "SELECT average_answer_time FROM statistics WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "failed to prepare statement" << std::endl;
        return -1;
    }

    // Bind username as parameter 1 to prevent sql injection
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    float time = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        time = static_cast<float>(sqlite3_column_double(stmt, 0));

    sqlite3_finalize(stmt);
    return time;
}

int SqliteDatabase::getNumOfAnswers(string username)
{
    string query = "SELECT total_answers FROM statistics WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "failed to prepare statement" << std::endl;
        return -1;
    }

    // Bind username as parameter 1 to prevent sql injection
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int num = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        num = static_cast<int>(sqlite3_column_double(stmt, 0));

    sqlite3_finalize(stmt);
    return num;
}

int SqliteDatabase::getNumOfCorrectAnswers(string username)
{
    string query = "SELECT correct_answers FROM statistics WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "failed to prepare statement" << std::endl;
        return -1;
    }

    // Bind username as parameter 1 to prevent sql injection
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int num = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        num = static_cast<int>(sqlite3_column_double(stmt, 0));

    sqlite3_finalize(stmt);
    return num;
}

int SqliteDatabase::getNumOfPlayerGames(string username)
{
    string query = "SELECT games_played FROM statistics WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "failed to prepare statement" << std::endl;
        return -1;
    }

    // Bind username as parameter 1 to prevent sql injection
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int num = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        num = static_cast<int>(sqlite3_column_double(stmt, 0));

    sqlite3_finalize(stmt);
    return num;
}

int SqliteDatabase::getPlayerScore(string username)
{
    string query = "SELECT total_score FROM statistics WHERE username = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(this->_db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "failed to prepare statement" << std::endl;
        return -1;
    }

    // Bind username as parameter 1 to prevent sql injection
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    int score = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        score = static_cast<int>(sqlite3_column_double(stmt, 0));

    sqlite3_finalize(stmt);
    return score;
}

vector<tuple<string, int>> SqliteDatabase::getHighScores()
{
    vector<tuple<string, int>> results;

    if (!_db)
    {
        std::cerr << "Database not open" << std::endl;
        return results;
    }

    string query = "SELECT username, total_score FROM statistics ORDER BY total_score DESC LIMIT 5;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(_db, query.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(_db) << std::endl;
        return results;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        const unsigned char* usernameText = sqlite3_column_text(stmt, 0);
        int highscore = sqlite3_column_int(stmt, 1);

        string username =  reinterpret_cast<const char*>(usernameText);
        results.emplace_back(username, highscore);
    }

    if (rc != SQLITE_DONE)
        std::cerr << "Failed during step: " << sqlite3_errmsg(_db) << std::endl;

    sqlite3_finalize(stmt);

    return results;
}