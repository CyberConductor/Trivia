#include "JsonRequestPacketDeserializer.h"
#include <stdexcept>

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
	if (buffer.size() < 5)
		throw std::invalid_argument("buffer too short");

	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
	if (buffer.size() < 5 + jsonSize)
		throw std::invalid_argument("json size mismatch");

	// Check if the message code matches
	if (buffer[0] != 1)
		throw std::invalid_argument("Invalid message code for LoginRequest");

	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	LoginRequest req;
	req.username = j["username"];
	req.password = j["password"];
	return req;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignUpRequest(const Buffer& buffer)
{
	if (buffer.size() < 5)
		throw std::invalid_argument("buffer too short");

	int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
	if (buffer.size() < 5 + jsonSize)
		throw std::invalid_argument("json size mismatch");

	// Check if the message code matches
	if (buffer[0] != 2)
		throw std::invalid_argument("Invalid message code for SignupRequest");

	string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
	json j = json::parse(jsonStr);

	SignupRequest req;
	req.username = j["username"];
	req.password = j["password"];
	req.email = j["email"];
	return req;
}
