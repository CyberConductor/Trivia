#pragma once

#include <string>
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler();

	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
};