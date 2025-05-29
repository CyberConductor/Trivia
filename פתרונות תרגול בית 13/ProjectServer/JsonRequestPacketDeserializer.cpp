#include "JsonRequestPacketDeserializer.h"
#include <stdexcept>

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	CheckErrors(Request_Login, buffer);
	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];

	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	LoginRequest req;
	req.username = j["username"];
	req.password = j["password"];
	return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
	CheckErrors(Request_Signup, buffer);
	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];

	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	SignupRequest req;
	req.username = j["username"];
	req.password = j["password"];
	req.email = j["email"];
	return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const Buffer& buffer)
{

	CheckErrors(Request_GetPlayersInRoom, buffer);
	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
	
	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	GetPlayersInRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{

	CheckErrors(Request_JoinRoom, buffer);
	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];

	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	JoinRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{
	CheckErrors(Request_CreateRoom, buffer);
	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];

	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	CreateRoomRequest req;
	req.roomName = j["roomName"];
	req.maxUsers = j["maxUsers"];
	req.questionCount = j["questionCount"];
	req.answerTimeOut = j["answerTimeOut"];
	return req;
}

void JsonRequestPacketDeserializer::CheckErrors(unsigned char code, const Buffer& buffer)
{
	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
	if (buffer.size() < 5 + jsonSize)
		throw std::invalid_argument("Json size mismatch");

	// Check if the message code matches
	if (buffer[0] != code)
		throw std::invalid_argument("Message code mismatch");
}

