#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room& room)
	: RoomMember(user, room,  factory.getRoomManager(), factory)
{}

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
		if ((RoomMemberRequestHandler*)user.second)
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeLeaveRoomResponse({ 1 });
			Helper::sendData(user.first.getSocket(), string(buffer.begin(), buffer.end()));
		}
		else if ((RoomAdminRequestHandler*)user.second)
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeLeaveRoomResponse({ 1 });
			Helper::sendData(user.first.getSocket(), string(buffer.begin(), buffer.end()));
		}
	}
	m_room.m_metadata.status = false;
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
		if (RoomMemberRequestHandler* handler = (RoomMemberRequestHandler*)user.second)
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
			Helper::sendData(user.first.getSocket(), string(buffer.begin(), buffer.end()));
			user.second = (IRequestHandler*)m_handlerFactory.createGameRequestHandler(m_user, m_room, handler);
		}
		else if ((RoomAdminRequestHandler*)user.second)
		{
			Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
			Helper::sendData(user.first.getSocket(), string(buffer.begin(), buffer.end()));
		}
	}
	m_room.m_metadata.status = true;
	//return response: 1
	Buffer buffer = JsonResponsePacketSerializer::serializeStartGameResponse({ 1 });
	return { buffer, (IRequestHandler*)m_handlerFactory.createGameRequestHandler(m_user, m_room, this) };
}