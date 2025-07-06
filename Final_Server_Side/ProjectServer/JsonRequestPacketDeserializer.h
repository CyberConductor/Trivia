#pragma once
#include <string>
#include <vector>
#include <ctime>
#include "json.hpp"

using std::string;
using std::vector;
using json = nlohmann::json;

using Buffer = vector<unsigned char>;

struct LoginRequest
{
	string username;
	string password;
};

struct SignupRequest
{
	string username;
	string password;
	string email;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	time_t answerTimeOut;
};

struct SubmitAnswerRequest
{
	unsigned int answerId;
	time_t anwserTime;
};

enum Requests : unsigned char
{
	//1
	Request_Login = 1,// skip 0 - error
	Request_Signup,
	//2
	Request_Signout,
	Request_GetRooms,
	Request_GetPlayersInRoom,
	Request_JoinRoom,
	Request_CreateRoom,
	Request_GetHighScore,
	Request_GetPersonalStats,
	//3
	Request_CloseRoom,
	Request_StartGame,
	Request_GetRoomState,
	Request_LeaveRoom,
	//4
	Request_LeaveGame,
	Request_GetQuestion,
	Request_SubmitAnswer,
	Request_GetGameResults
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