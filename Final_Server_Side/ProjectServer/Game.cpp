#include "Game.h"
#include <algorithm>
#include <random>
#include <iostream>

Game::Game(Room& room, vector<Question> questions)
    : m_room(room), m_questions(questions), m_gameId(room.m_metadata.id)
{
    std::shuffle(m_questions.begin(), m_questions.end(), std::mt19937(std::random_device{}()));

    for (const auto& user : room.m_users)
    {
        m_players.emplace(user.first, GameData{ questions[0], 0, 0, 0 });
    }

    totalPlayers = static_cast<int>(m_players.size());
}

Question Game::getQuestionForUser(LoggedUser user)
{
    lock_guard<mutex> lock(mtx);
    return m_players.at(user).currentQuestion;
}

bool Game::submitAnswer(LoggedUser user, int answerId, time_t answerTime)
{
    unique_lock<mutex> lock(mtx);

    auto& data = m_players.at(user);
    bool result = updateScore(data, answerTime, answerId);

    answersReceived++;

    if (answersReceived < totalPlayers)
    {
        cv.wait(lock, [this]() { return answersReceived == 0; });
    }
    else
    {
        answersReceived = 0;
        cv.notify_all();

        // advance to next question
        if (!m_questions.empty())
        {
            m_questions.erase(m_questions.begin());

            for (auto& player : m_players)
                player.second.currentQuestion = m_questions[0];
        }
    }

    return result;
}

bool Game::removePlayer(string username)
{
    lock_guard<mutex> lock(mtx);

    for (auto it = m_players.begin(); it != m_players.end(); ++it)
    {
        if (it->first.getUsername() == username)
        {
            m_players.erase(it);
            totalPlayers--;
            return true;
        }
    }
    return false;
}

bool Game::updateScore(GameData& data, time_t answerTime, unsigned int answerId)
{
    int oldTotal = data.correctAnswerCount + data.wrongAnswerCount;
    data.avarageAnswerTime = ((data.avarageAnswerTime * oldTotal) + answerTime) / (oldTotal + 1);

    if (data.currentQuestion.getCorrectAnswerId() == answerId)
    {
        data.correctAnswerCount++;
        return true;
    }

    data.wrongAnswerCount++;
    return false;
}

bool Game::operator==(const Game& other) const
{
    return m_gameId == other.m_gameId;
}

bool Game::allPlayersAnswered()
{
    for (auto player : m_players)
    {
        if (player.second.currentQuestion == m_questions[0])
            return false;
    }
    return true;
}
