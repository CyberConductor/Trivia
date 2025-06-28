#include "RoomAdminRequestHandler.h"
#include "IRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room& room)
	: RoomMember(user, room, factory.getRoomManager(), factory)
{}

bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo req)
{
	return (req.id >= Request_CloseRoom && req.id <= Request_GetRoomState);
}

RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo req)
{
	if (!isRequestRelevant(req))
		return generateErrorResponse("Unrecognized request type", this);

	switch (req.id)
	{
	case Request_CloseRoom:
		return closeRoom(req);
	case Request_StartGame:
		return startGame(req);
	case Request_GetRoomState:
		return getRoomState(req);
	default:
		return generateErrorResponse("Unhandled request type", this);
	}
}

RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo requestInfo)
{
	for (auto& user : m_room.m_users)
	{
		// send LeaveRoomResponse for each user
		if (dynamic_cast<RoomMemberRequestHandler*>(user.second) ||
			dynamic_cast<RoomAdminRequestHandler*>(user.second))
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeLeaveRoomResponse({ 1 });
			Helper::sendData(user.first.getSocket(), buffer);
		}
	}

	m_room.m_metadata.status = false;
	m_roomManager.deleteRoom(m_room.m_metadata.id);

	// return response: 1
	Buffer buffer = JsonResponsePacketSerializer::serializeCloseRoomResponse({ 1 });
	return { buffer, m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomAdminRequestHandler::startGame(RequestInfo)
{
	for (auto& user : m_room.m_users)
	{
		if (RoomMemberRequestHandler* memberHandler = dynamic_cast<RoomMemberRequestHandler*>(user.second))
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
			Helper::sendData(user.first.getSocket(), buffer);

			// convert to game request handler
			user.second = m_handlerFactory.createGameRequestHandler(user.first, m_room, memberHandler);
		}
	}

	m_room.m_metadata.status = true;

	// return response for the admin
	Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
	return { buffer, m_handlerFactory.createGameRequestHandler(m_user, m_room, this) };
}
