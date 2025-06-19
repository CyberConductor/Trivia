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
