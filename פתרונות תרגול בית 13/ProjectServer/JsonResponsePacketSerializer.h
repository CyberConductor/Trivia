#pragma once

#include <vector>
#include "json.hpp"
#include "LoginResponseHandler.h"
#include "SignupResponse.h"
#include "ErrorResponse.h"

using std::vector;

static class JsonResponsePacketSerializer
{
public:
	JsonResponsePacketSerializer();
	~JsonResponsePacketSerializer();

	void serializeLoginResponse();
	void serializeSignUpResponse();
	void serializeErrorResponse();

private:

};