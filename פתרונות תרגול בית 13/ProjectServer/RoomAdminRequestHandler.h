#pragma once
#include "RoomMember.h"
#include "RoomMemberRequestHandler.h"
#include "Communicator.h"
#include "Helper.h"

class RoomAdminRequestHandler : public RoomMember
{
public:
	//Constructor
	RoomAdminRequestHandler(RequestHandlerFactory&, LoggedUser, Room, Communicator*);
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
private:
	//Methods
	RequestResult closeRoom(RequestInfo);
	RequestResult startGame(RequestInfo);
	//attributes
	Communicator* m_communicator;
};