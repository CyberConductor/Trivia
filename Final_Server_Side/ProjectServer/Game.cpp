#include "Game.h"
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "Question.h"
#include "Room.h"
#include <algorithm>
#include <random>
#include "StatisticsManager.h"

Game::Game(Room& room, vector<Question> questions)
    : m_room(room),
    m_questions(questions),
    m_gameId(room.m_metadata.id) 
{
    //shuffle the questions order
    std::shuffle(m_questions.begin(), m_questions.end(), std::mt19937(std::random_device{}()));

    for (auto user : room.m_users)
        m_players.emplace(
            user.first,
            GameData({ questions[0], 0, 0, 0 })
        );
}

Question Game::getQuestionForUser(LoggedUser user)
{
    return m_players.find(user)->second.currentQuestion;
}

void Game::submitAnswer()
{
    m_questions.erase(m_questions.begin());
    if (!m_questions.empty())
    {
        for (auto& player : m_players)
            player.second.currentQuestion = m_questions[0];
    }
}

bool Game::removePlayer(string username)
{
    for (auto it = m_players.begin(); it != m_players.end(); ++it)
    {
        if (it->first.getUsername() == username)
        {
            m_players.erase(it);
            return true;
        }
    }
    return false; // player not found
}

void Game::updateScore(string username, time_t answerTime)
{
    for (auto it = m_players.begin(); it != m_players.end(); ++it)
    {
        if (it->first.getUsername() == username)
        {
            int oldToatal = (it->second.correctAnswerCount++) + it->second.wrongAnswerCount;
            it->second.avarageAnswerTime = ((it->second.avarageAnswerTime * oldToatal) + answerTime) / (oldToatal + 1);
        }
    }
}

bool Game::operator==(const Game& other) const
{
    return m_gameId == other.m_gameId;
}