#include "GameManager.h"
#include "JsonResponsePacketSerializer.h"
#include "curl-8.14.1/include/curl/curl.h"

GameManager::GameManager(IDatabase* database)
	: m_database(database){}

Game& GameManager::createGame(Room room)
{
    // check if a game for this room already exists
    for (Game& game : m_games)
    {
        if (room.m_metadata.id == game.m_gameId)
            throw std::runtime_error("game for this room already exists");
    }

    // get questions from database. TODO: get questions from API
    vector<Question> questions = m_database->getQuestion(room.m_metadata.numOfQuestionsInGame);

    // create the game
    Game game(room, questions);

    // add the game to the list
    m_games.push_back(game);

    // return a reference to the last game in the vector
    return m_games.back();
}


void GameManager::deleteGame(int id)
{
	for (auto it = m_games.begin(); it != m_games.end(); ++it)
	{
		if (id == it->m_gameId)
		{
			for (auto player : it->m_players)
				it->removePlayer(player.first.getUsername());
			m_games.erase(it);
		}
	}
}

int GameManager::submitGameStatsToDB(GameData data)
{
	StatisticsManager statsManager = StatisticsManager((SqliteDatabase*)m_database);
	for (auto game : m_games)
		for (auto player : game.m_players)
			if (player.second == data)
				return statsManager.submitGameStatistics(player.first.getUsername(), data);
	return SQLITE_ERROR;
}

size_t GameManager::WriteCallback(void* contents, size_t size, size_t nmemb, string* output)
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

vector<Question> GameManager::fetchQuestionsFromAPI(const RoomData& room)
{
    vector<Question> questions;
    string url = "https://opentdb.com/api.php?amount=" + std::to_string(room.numOfQuestionsInGame) + "&type=multiple&encode=url3986";

    CURL* curl = curl_easy_init();
    if (!curl)
        return questions;

    string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return questions;

    json data = json::parse(response);
    if (data["response_code"] != 0)
        return questions;

    for (const auto& item : data["results"])
    {
        string rawQ = item["question"];
        string rawCorrect = item["correct_answer"];
        vector<string> rawIncorrect = item["incorrect_answers"].get<vector<string>>();

        char* decodedQ_ptr = curl_unescape(rawQ.c_str(), 0);
        char* decodedCorrect_ptr = curl_unescape(rawCorrect.c_str(), 0);
        string decodedQ = decodedQ_ptr;
        string decodedCorrect = decodedCorrect_ptr;
        curl_free(decodedQ_ptr);
        curl_free(decodedCorrect_ptr);

        vector<string> allAnswers;
        for (const auto& wrong : rawIncorrect)
        {
            char* decoded_ptr = curl_unescape(wrong.c_str(), 0);
            allAnswers.push_back(decoded_ptr);
            curl_free(decoded_ptr);
        }

        int correctPos = rand() % (allAnswers.size() + 1);
        allAnswers.insert(allAnswers.begin() + correctPos, decodedCorrect);

        questions.emplace_back(decodedQ, decodedCorrect, allAnswers);
    }

    return questions;
}