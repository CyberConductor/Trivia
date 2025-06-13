#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory()
    : m_roomManager(RoomManager(this)),
    m_database(new SqliteDatabase()),
    m_statisticsManager(m_database)
{
    m_database->open();
}

RequestHandlerFactory::~RequestHandlerFactory() { delete m_database; }

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler(SOCKET sock) { return new LoginRequestHandler(*this, sock); }
IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user) 
{ return new MenuRequestHandler(*this, user); }

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room)
{
    return new RoomAdminRequestHandler(*this, user, room);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room room)
{
    return new RoomMemberRequestHandler(*this, user, room);
}

LoginManager& RequestHandlerFactory::getLoginManager() { return m_loginManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return m_statisticsManager; }
RoomManager& RequestHandlerFactory::getRoomManager() { return m_roomManager; }