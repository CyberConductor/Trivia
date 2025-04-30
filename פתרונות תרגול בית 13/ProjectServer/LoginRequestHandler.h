#pragma once

#include <string>
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo) override;
	virtual RequestResult handleRequest(RequestInfo) override;
};