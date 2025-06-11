#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "Communicator.h"

RequestHandlerFactory::RequestHandlerFactory(Communicator* communicator)
    : m_roomManager(RoomManager(this)),
    m_database(new SqliteDatabase()),
    m_statisticsManager(m_database),
    m_communicator(communicator)
{
    m_database->open();
}

RequestHandlerFactory::~RequestHandlerFactory() { delete m_database; }

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() { return new LoginRequestHandler(*this); }
IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user) 
{ return new MenuRequestHandler(*this, user); }

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room room)
{
    return new RoomAdminRequestHandler(*this, user, room, m_communicator);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room room)
{
    return new RoomMemberRequestHandler(*this, user, room);
}

LoginManager& RequestHandlerFactory::getLoginManager() { return m_loginManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return m_statisticsManager; }
RoomManager& RequestHandlerFactory::getRoomManager() { return m_roomManager; }