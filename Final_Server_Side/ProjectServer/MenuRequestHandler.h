<<<<<<< HEAD:Final_Server_Side/ProjectServer/MenuRequestHandler.h
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
	MenuRequestHandler(RequestHandlerFactory&, LoggedUser);
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
	RequestResult signout(RequestInfo);
	RequestResult getRooms(RequestInfo);
	RequestResult getPlayersInRoom(RequestInfo);
	RequestResult getPersonalStats(RequestInfo);
	RequestResult getHighScore(RequestInfo);
	RequestResult joinRoom(RequestInfo);
	RequestResult createRoom(RequestInfo);

	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
};
=======
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
>>>>>>> b2ee5add6bbab06d883a999d61c983eff6764292:פתרונות תרגול בית 13/ProjectServer/MenuRequestHandler.h
