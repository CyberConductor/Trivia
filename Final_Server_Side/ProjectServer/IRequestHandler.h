#pragma once

#include <vector>
#include <string>
#include <ctime> 
#include <iostream>
#include "JsonResponsePacketSerializer.h" 

using std::vector;
using std::string;

typedef vector<unsigned char> Buffer;

struct RequestInfo
{
	int id;
	time_t receivalTime;
	Buffer buffer;
};

class IRequestHandler;// RequestResult uses IRequestHandler before it has been defined
struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler = nullptr;
};

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;
	static RequestResult generateErrorResponse(string str, IRequestHandler* handler)
	{
		return { JsonResponsePacketSerializer::serializeErrorResponse({ str }), handler };
	}
};