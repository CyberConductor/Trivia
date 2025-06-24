#pragma once
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory&);
	//virtual functions

	virtual bool isRequestRelevant(RequestInfo) override;
	virtual RequestResult handleRequest(RequestInfo) override;
private:
	//Methods
	RequestResult login(RequestInfo);
	RequestResult signup(RequestInfo);
	//fields
	RequestHandlerFactory& m_handlerFactory;
	LoginManager& m_loginManager;
};