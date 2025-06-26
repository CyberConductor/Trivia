#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RoomMember : public IRequestHandler
{
public:
	//Constructor and Destructor
	RoomMember(LoggedUser user, Room& room, RoomManager& manager, RequestHandlerFactory& factory)
		: m_user(user),
		m_room(room),
		m_roomManager(manager),
		m_handlerFactory(factory)
	{}
	//virtual functions
	virtual bool isRequestRelevant(RequestInfo) = 0;
	virtual RequestResult handleRequest(RequestInfo) = 0;
protected:
	//Methods
	virtual RequestResult getRoomState(RequestInfo)
	{
		GetRoomStateResponse state = 
		{ 
			1, 
			m_room.m_metadata.status,
			m_room.getAllUsers(),
			m_room.m_metadata.numOfQuestionsInGame,
			m_room.m_metadata.timePerQuestion 
		};
		Buffer buffer = JsonResponsePacketSerializer::serializeGetRoomStateResponse({ state });
		return { buffer , this };
	}
	//attributes
	Room& m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;
};
