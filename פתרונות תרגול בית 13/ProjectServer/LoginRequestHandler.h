#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& factory); 

	//virtual functions
	virtual bool isRequestRelevant(RequestInfo) override;
	virtual RequestResult handleRequest(RequestInfo) override;
	RequestResult login(RequestInfo);
	RequestResult signup(RequestInfo);
private:
	RequestHandlerFactory& m_handlerFactory;
	LoginManager& m_loginManager;
};