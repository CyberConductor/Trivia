#pragma once
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "LoginManager.h"
#include "Question.h"

using std::string;
using std::vector;
using std::map;

typedef struct GameData {
	Question currentQuestion;
	std::time_t correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int avarageAnswerTime;
};

class Game
{
public:
	Question getQuestionForUser(LoggedUser user);

private:
	vector<Question> m_questions;
	map<LoggedUser, GameData> m_players;
	unsigned int m_gameId;
};