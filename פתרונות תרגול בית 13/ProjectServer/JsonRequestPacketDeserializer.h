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

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignUpRequest(const Buffer& buffer);
};