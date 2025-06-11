#pragma once
#include "RoomMember.h"
#include "Room.h"

class RoomMemberRequestHandler : public RoomMember
{
public:
	//Constructor
	RoomMemberRequestHandler(RequestHandlerFactory&, LoggedUser&, Room);
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
	RequestResult leaveRoom(RequestInfo);
};