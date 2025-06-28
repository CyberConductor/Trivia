#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <ctime>
#include "json.hpp"
#include "Room.h"
#include "StatisticsManager.h"

using json = nlohmann::json;
using std::string;
using std::vector;
using std::map;

using Buffer = vector<unsigned char>;

struct ErrorResponse
{
	string message;
};

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct LogoutResponse
{
	unsigned int status;
};

struct GetRoomsResponse
{
	unsigned int status;
	vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	vector<string> players;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
	unsigned int id;
};

struct getHighScoreResponse
{
	unsigned int status;
	map<string, int> statistics;
};

struct getPersonalStatsResponse
{
	unsigned int status;
	json statistics;
};

struct CloseRoomResponse
{
	unsigned int status;
};

struct StartGameResponse
{
	unsigned int status;
};

struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	time_t answerTimeOut;
};

struct LeaveRoomResponse
{
	unsigned int status;
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse
{
	unsigned int status;
	string question;
	map<unsigned int, string> answers;
};

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnwserId;
};

struct PlayerResults
{
	string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswersCount;
	time_t avarageAnswerTime;
};

struct GetGameResultsResponse
{
	unsigned int status;
	vector<PlayerResults> results;
};

enum Responses : unsigned char
{
	//1
	Response_Error = 0,
	Response_Login,
	Response_Signup,
	//2
	Response_Logout,
	Response_GetRooms,
	Response_GetPlayersInRoom,
	Response_JoinRoom,
	Response_CreateRoom,
	Response_GetHighScore,
	Response_GetPersonalStatus,
	//3
	Response_CloseRoom,
	Response_StartGame,
	Response_GetRoomState,
	Response_LeaveRoom,
	//4
	Response_LeaveGame,
	Response_GetQuestion,
	Response_SubmitAnswer,
	Response_GetGameResults
};

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeErrorResponse(ErrorResponse);
	static Buffer serializeLoginResponse(LoginResponse);
	static Buffer serializeSignupResponse(SignupResponse);
	static Buffer serializeLogoutResponse(LogoutResponse);
	static Buffer serializeGetRoomsResponse(GetRoomsResponse);
	static Buffer serializeGetPlayersInRoomResponse(GetPlayersInRoomResponse);
	static Buffer serializeJoinRoomResponse(JoinRoomResponse);
	static Buffer serializeCreateRoomResponse(CreateRoomResponse);
	static Buffer serializegetHighScoreResponse(getHighScoreResponse);
	static Buffer serializegetPersonalStatsResponse(getPersonalStatsResponse);
	static Buffer serializeCloseRoomResponse(CloseRoomResponse);
	static Buffer serializeStartGameResponse(StartGameResponse);
	static Buffer serializeGetRoomStateResponse(GetRoomStateResponse);
	static Buffer serializeLeaveRoomResponse(LeaveRoomResponse);
	static Buffer serializeLeaveGameResponse(LeaveGameResponse);
	static Buffer serializeGetQuestionResponse(GetQuestionResponse);
	static Buffer serializeSubmitAnswerResponse(SubmitAnswerResponse);
	static Buffer serializeGetGameResultsResponse(GetGameResultsResponse);
private:
	static Buffer buildResponseBuffer(unsigned char code, json& j);
};