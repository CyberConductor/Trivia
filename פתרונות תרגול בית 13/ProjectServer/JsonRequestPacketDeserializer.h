#pragma once
#include <string>
#include <vector>
#include "json.hpp"

using std::string;
using std::vector;
using json = nlohmann::json;

typedef vector<unsigned char> Buffer;

typedef struct LoginRequest
{
	string username;
	string password;
}LoginRequest;

typedef struct SignupRequest
{
	string username;
	string password;
	string email;
}SignupRequest;

typedef struct GetPlayersInRoomRequest
{
	unsigned int roomId;
}GetPlayersInRoomRequest;

typedef struct JoinRoomRequest
{
	unsigned int roomId;
}JoinRoomRequest;

typedef struct CreateRoomRequest
{
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeOut;
}CreateRoomRequest;

typedef struct SubmitAnswerRequest
{
	unsigned int answerId;
}SubmitAnswerRequest;

enum Requests : unsigned char
{
	Request_Login = 0,
	Request_Signup,
	Request_Signout,
	Request_GetRooms,
	Request_GetPlayersInRoom,
	Request_GetPersonalStats,
	Request_GetHighScore,
	Request_JoinRoom,
	Request_CreateRoom,
	Request_CloseRoom,
	Request_StartGame,
	Request_GetRoomState,
	Request_LeaveRoom,
	Request_SubmitAnswer
};

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const Buffer&);
	static SignupRequest deserializeSignupRequest(const Buffer&);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const Buffer&);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer&);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer&);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const Buffer&);
	//Helpers
	static void CheckErrors(unsigned char, const Buffer&);
};