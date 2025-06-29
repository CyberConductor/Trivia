#include "GameManager.h"
#include "JsonResponsePacketSerializer.h"
//#include "curl/curl.h"

GameManager::GameManager(IDatabase* database)
	: m_database(database){}

GameManager::~GameManager()
{
    for (Game* game : m_games)
        delete game;
    m_games.clear();
}

Game& GameManager::createGame(Room& room)
{
    // check if a game for this room already exists
    for (Game* game : m_games)
    {
        if (room.m_metadata.id == game->m_gameId)
            return *game;
    }

    // get questions from database. TODO: get questions from API
    unsigned int amount = room.m_metadata.numOfQuestionsInGame;
    vector<Question> questions = m_database->getQuestion(amount);
    if (questions.size() < amount)
    {
        vector<Question> fromAPI = fetchQuestionsFromAPI(room.m_metadata);
        unsigned int need = amount - questions.size();
        if (fromAPI.size() < need)
            throw std::runtime_error("not enough questions in DB and API to start game");

        questions.insert(questions.end(), fromAPI.begin(), fromAPI.begin() + need);
    }
    // create the game
    Game* game = new Game(room, questions);

    // add the game to the list
    m_games.push_back(game);

    return *game;
}


void GameManager::deleteGame(int id)
{
	for (auto it = m_games.begin(); it != m_games.end(); ++it)
	{
		if (id == (*it)->m_gameId)
		{
            for (auto player : (*it)->m_players)
            {
                auto username = player.first.getUsername();
                submitGameStatsToDB(username, player.second);
                (*it)->removePlayer(username);
            }

            delete *it;
			m_games.erase(it);
            break;
		}
	}
}

int GameManager::submitGameStatsToDB(string username, GameData data)
{
	StatisticsManager statsManager = StatisticsManager(dynamic_cast<SqliteDatabase*>(m_database));

    return statsManager.submitGameStatistics(username, data);
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