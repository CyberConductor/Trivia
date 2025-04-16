#pragma once

#include <string>
#include "IRequestHandler.h"

class LoginRequestHandler : IRequestHandler
{
public:
	LoginRequestHandler();
	LoginRequestHandler(std::string);
	~LoginRequestHandler();
};