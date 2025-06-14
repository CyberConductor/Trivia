#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room room)
	: RoomMember(user, room,  factory.getRoomManager(), factory) {}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo req)
{
	return (req.id >= Request_CloseRoom && req.id <= Request_GetRoomState);
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo req)
{
	if(!isRequestRelevant(req))
		return generateErrorResponse("Unrecognized request type", this);
	switch (req.id)
	{
	case Request_CloseRoom:
		return closeRoom(req);
	case Request_StartGame:
		return startGame(req);
	case Request_GetRoomState:
		return getRoomState(req);
	}
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo requestInfo)
{
	for (auto& user : m_room.m_users)
	{
		//send LeaveRoomRespone for each user
		if (RoomMemberRequestHandler* handler = dynamic_cast<RoomMemberRequestHandler*>(user.second))
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeLeaveRoomResponse({ 1 });
			//get client socket
			for (auto client : m_room.m_users)
				if(handler == client.second)
				{
					Helper::sendData(client.first.getSocket(), string(buffer.begin(), buffer.end()));
					break;
				}
		}
		else if (RoomAdminRequestHandler* handler = dynamic_cast<RoomAdminRequestHandler*>(user.second))
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeLeaveRoomResponse({ 1 });
			//find the admin socket
			for (auto client : m_room.m_users)
				if (handler == this)
				{
					Helper::sendData(client.first.getSocket(), string(buffer.begin(), buffer.end()));
					break;
				}
		}
	}
	m_roomManager.deleteRoom(m_room.m_metadata.id);

	//return response: 1
	Buffer buffer = JsonResponsePacketSerializer::serializeCloseRoomResponse({ 1 });
	return { buffer, m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	for (auto& user : m_room.m_users)
	{
		//send StartGameRespone for each user
		if (RoomMemberRequestHandler* handler = dynamic_cast<RoomMemberRequestHandler*>(user.second))
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
			//get client socket
			for (auto client : m_room.m_users)
				if (handler == client.second)
				{
					Helper::sendData(client.first.getSocket(), string(buffer.begin(), buffer.end()));
					break;
				}
		}
		else if (RoomAdminRequestHandler* handler = dynamic_cast<RoomAdminRequestHandler*>(user.second))
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
			//find the admin socket
			for (auto client : m_room.m_users)
				if (handler == this)
				{
					Helper::sendData(client.first.getSocket(), string(buffer.begin(), buffer.end()));
					break;
				}
		}
	}
	//return response: 1
	Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
	return { buffer, m_handlerFactory.createMenuRequestHandler(m_user) };
}

