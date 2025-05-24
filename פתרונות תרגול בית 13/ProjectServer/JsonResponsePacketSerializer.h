#pragma once

#include <string>
#include <vector>
#include "json.hpp"
#include "Room.h"

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
	vector<string> statistics;
}getHighScoreResponse;

typedef struct getPersonalStatsResponse
{
	unsigned int status;
	vector<string> statistics;
}getPersonalStatsResponse;

static enum Responses : unsigned char
{
	Error = 0,
	Login,
	Signup,
	Logout,
	GetRooms,
	GetPlayersInRoom,
	JoinRoom,
	CreateRoom,
	GetHighScore,
	GetPersonalStatus
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