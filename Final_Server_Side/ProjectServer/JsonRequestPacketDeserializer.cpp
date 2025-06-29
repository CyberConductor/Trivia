#include "JsonRequestPacketDeserializer.h"
#include <stdexcept>
#include <iostream>

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));

	LoginRequest req;
	req.username = j["username"];
	req.password = j["password"];
	return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));

	SignupRequest req;
	req.username = j["username"];
	req.password = j["password"];
	req.email = j["email"];
	return req;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const Buffer& buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));

	GetPlayersInRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));

	JoinRoomRequest req;
	req.roomId = j["roomId"];
	return req;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));

	CreateRoomRequest req;
	req.roomName = j["roomName"];
	req.maxUsers = j["maxUsers"];
	req.questionCount = j["questionCount"];
	req.answerTimeOut = j["answerTimeOut"];
	return req;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const Buffer& buffer)
{
	json j = json::parse(string(buffer.begin(), buffer.end()));

	SubmitAnswerRequest req;
	req.answerId = j["answerId"];
	req.answerId = j["timeTaken"];

	return req;
}