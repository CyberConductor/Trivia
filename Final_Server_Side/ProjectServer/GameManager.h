#pragma once
#include "curl2/curl.h"

#ifdef _DEBUG
#pragma comment (lib, "curl2/libcurl_a_debug.lib")
#else 
#pragma comment (lib, "curl2/libcurl_a.lib")
#endif // _DEBUG

#include "Room.h"
#include "Game.h"
#include "StatisticsManager.h"

class GameManager
{
public:
	//Constructor
	GameManager(IDatabase*);
	//methods
	Game& createGame(Room);
	void deleteGame(int);
	int submitGameStatsToDB(GameData);
private:
	//helpers
	static size_t WriteCallback(void*, size_t, size_t, string*);
	vector<Question> fetchQuestionsFromAPI(const RoomData&);

	vector<Game> m_games;
	IDatabase* m_database;
};