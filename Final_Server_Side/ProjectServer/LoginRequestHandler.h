#pragma once
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

class LoginRequestHandler : public IRequestHandler
{
public:
<<<<<<< HEAD:Final_Server_Side/ProjectServer/LoginRequestHandler.h
	LoginRequestHandler(RequestHandlerFactory&, SOCKET);
=======
	LoginRequestHandler(RequestHandlerFactory&);
>>>>>>> b2ee5add6bbab06d883a999d61c983eff6764292:פתרונות תרגול בית 13/ProjectServer/LoginRequestHandler.h
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
	SOCKET m_socket;
};