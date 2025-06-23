#pragma once
#include <exception>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

using std::exception;

class MenuRequestHandler : public IRequestHandler
{
public:
	//Constructor
	MenuRequestHandler();
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
};
