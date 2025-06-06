#include "RoomAdminRequestHandler.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& factory, LoggedUser& user)
	: m_handlerFactory(factory), m_user(user), m_roomManager(factory.getRoomManager()) 
{
	
}
