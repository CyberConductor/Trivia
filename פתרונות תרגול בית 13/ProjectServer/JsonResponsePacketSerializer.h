#pragma once

#include <string>
#include <vector>
#include "json.hpp"
#include "Room.h"
#include "StatisticsManager.h"

using json = nlohmann::json;
using std::string;
using std::vector;

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
	Response_GetPersonalStatus
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

private:
	static Buffer buildResponseBuffer(unsigned char code, json& j);
};