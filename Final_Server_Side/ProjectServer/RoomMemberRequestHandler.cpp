#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& factory, LoggedUser user, Room& room)
	: RoomMember(user, room, factory.getRoomManager(), factory)
{}

bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo req)
{
	return (req.id >= Request_GetRoomState && req.id <= Request_LeaveRoom);
}

RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo req)
{
	if (!isRequestRelevant(req))
		return generateErrorResponse("Unrecognized request type", this);
	switch (req.id)
	{
	case Request_GetRoomState:
		return getRoomState(req);
	case Request_LeaveRoom:
		return leaveRoom(req);
	}
}

RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo)
{
	m_room.removeUser(m_user);

	Buffer buffer = JsonResponsePacketSerializer::serializeLeaveRoomResponse({ 1 });
	return RequestResult({ buffer, m_handlerFactory.createMenuRequestHandler(m_user) });
}