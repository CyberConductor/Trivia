#pragma once
//#include "RoomMember.h"
#include "RoomMemberRequestHandler.h"
#include "Helper.h"

class RoomAdminRequestHandler : public RoomMember
{
public:
	//Constructor
	RoomAdminRequestHandler(RequestHandlerFactory&, LoggedUser, Room);
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
	RequestResult closeRoom(RequestInfo);
	RequestResult startGame(RequestInfo);
};