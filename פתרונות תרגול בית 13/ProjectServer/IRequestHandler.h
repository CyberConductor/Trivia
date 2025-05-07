#pragma once

#include <vector>

using std::vector;

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
};

struct RequestResult
{
	Buffer response;
	IRequestHandler* newHandler;
};