#pragma once

#include <string>
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& factory); 

	//virtual functions
	virtual bool isRequestRelevant(RequestInfo) override;
	virtual RequestResult handleRequest(RequestInfo) override;
	RequestResult login(RequestInfo requestInfo);
	RequestResult signup(RequestInfo requestInfo);
private:
	RequestHandlerFactory& m_handlerFactory;
};