#pragma once

#include <vector>
#include <string>
#include <ctime> 
#include "JsonResponsePacketSerializer.h" 

using std::vector;
using std::string;

typedef vector<unsigned char> Buffer;

typedef struct RequestInfo
{
	int id;
	time_t receivalTime;
	Buffer buffer;
}RequestInfo;

typedef struct RequestResult RequestResult;

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;
<<<<<<< HEAD:פתרונות תרגול בית 13/ProjectServer/IRequestHandler.h
	static RequestResult generateErrorResponse(string, IRequestHandler*);
};

struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler = nullptr;
};

RequestResult IRequestHandler::generateErrorResponse(string str, IRequestHandler* handler)
{
	return { JsonResponsePacketSerializer::serializeErrorResponse({ str }), handler };
}
=======
	static RequestResult generateErrorResponse(string str, IRequestHandler* handler)
	{
		return { JsonResponsePacketSerializer::serializeErrorResponse({ str }), handler };
	}
};
>>>>>>> origin/develop:Final_Server_Side/ProjectServer/IRequestHandler.h
