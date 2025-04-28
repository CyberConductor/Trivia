#pragma once

#include <string>
#include <vector>
#include "json.hpp"

using std::string;
using std::vector;

typedef vector<unsigned char> Buffer;

typedef struct LoginResponse
{
	unsigned int status;
}LoginResponse;

typedef struct SignupResponse
{
	unsigned int status;
}SignupResponse;

typedef struct ErrorResponse
{
	string massage;
}ErrorResponse;


using std::vector;

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(ErrorResponse);
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
};