#pragma once
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include <mutex>
#include <condition_variable>
#include "Question.h"
#include "LoginManager.h"
#include "Room.h"

using std::string;
using std::vector;
using std::map;

using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::lock_guard;

class GameManager;
class IRequestHandler;
class GameRequestHandler;

struct GameData {
    Question currentQuestion;
    unsigned int correctAnswerCount;
    unsigned int wrongAnswerCount;
    time_t avarageAnswerTime;

    bool operator==(const GameData& other) const
    {
        return (correctAnswerCount == other.correctAnswerCount &&
            wrongAnswerCount == other.wrongAnswerCount &&
            avarageAnswerTime == other.avarageAnswerTime);
    }
};

class Game
{
public:
    Game(Room&, vector<Question>);
    Question getQuestionForUser(LoggedUser);
    bool submitAnswer(LoggedUser user, int answerId, time_t answerTime);
    bool removePlayer(string);
    bool updateScore(GameData&, time_t, unsigned int);
    bool operator==(const Game&) const;

private:
    bool allPlayersAnswered();

    vector<Question> m_questions;
    map<LoggedUser, GameData> m_players;
    unsigned int m_gameId;
    Room& m_room;

    mutex mtx;
    condition_variable cv;
    unsigned int answersReceived = 0;
    unsigned int totalPlayers = 0;

    friend GameManager;
    friend GameRequestHandler;
};
