#pragma once
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "Question.h"
#include "LoginManager.h"
#include "Room.h"


using std::string;
using std::vector;
using std::map;

//forward decleration
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
	//Constructor
	Game(Room, vector<Question>);
	//methods
	Question getQuestionForUser(LoggedUser);
	void submitAnswer();
	bool removePlayer(string);
	void updateScore(string, time_t);

	bool operator==(const Game&) const;
private:
	vector<Question> m_questions;
	map<LoggedUser, GameData> m_players;
	unsigned int m_gameId;
	Room m_room;

	friend GameManager;
	friend GameRequestHandler;
};