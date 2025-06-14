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

typedef vector<unsigned char> Buffer;

typedef struct ErrorResponse
{
	string message;
}ErrorResponse;

typedef struct LoginResponse
{
	unsigned int status;
}LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
}SignupResponse;

typedef struct LogoutResponse
{
	unsigned int status;
}LogoutResponse;

typedef struct GetRoomsResponse
{
	unsigned int status;
	vector<RoomData> rooms;
}GetRoomsResponse;

typedef struct GetPlayersInRoomResponse
{
	vector<string> players;
}GetPlayersInRoomResponse;

typedef struct JoinRoomResponse
{
	unsigned int status;
}JoinRoomResponse;

typedef struct CreateRoomResponse
{
	unsigned int status;
}CreateRoomResponse;

typedef struct getHighScoreResponse
{
	unsigned int status;
	json statistics;
}getHighScoreResponse;

typedef struct getPersonalStatsResponse
{
	unsigned int status;
	json statistics;
}getPersonalStatsResponse;

typedef struct CloseRoomResponse
{
	unsigned int status;
}CloseRoomResponse;

typedef struct StartGameResponse
{
	unsigned int status;
}StartGameResponse;

typedef struct GetRoomStateResponse
{
	unsigned int status;
	bool hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	std::time_t answerTimeOut;
}GetRoomStateResponse;

typedef struct LeaveRoomResponse
{
	unsigned int status;
}LeaveRoomResponse;

typedef struct LeaveGameResponse
{
	unsigned int status;
}LeaveGameResponse;

typedef struct GetQuestionResponse
{
	unsigned int status;
	string question;
	map<unsigned int, string> answers;
}GetQuestionResponse;

typedef struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnwserId;
}SubmitAnswerResponse;

typedef struct PlayerResults
{
	string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswersCount;
	std::time_t avarageAnswerTime;
}PlayerResults;

typedef struct GetGameResultsResponse
{
	unsigned int status;
	vector<PlayerResults> results;
}GetGameResultsResponse;


enum Responses : unsigned char
{
	Response_Error = 0,
	Response_Login,
	Response_Signup,
	Response_Logout,
	Response_GetRooms,
	Response_GetPlayersInRoom,
	Response_JoinRoom,
	Response_CreateRoom,
	Response_GetHighScore,
	Response_GetPersonalStatus,
	Response_CloseRoom,
	Response_StartGame,
	Response_GetRoomState,
	Response_LeaveRoom,
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