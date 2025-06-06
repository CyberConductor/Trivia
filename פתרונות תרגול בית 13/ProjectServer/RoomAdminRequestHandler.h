#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	//Constructor
	RoomAdminRequestHandler(RequestHandlerFactory&, LoggedUser&);
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
	
	//attributes
	Room m_room;
	LoggedUser m_user;
	RoomManager m_roomManager;
	RequestHandlerFactory& m_handlerFactory;
};