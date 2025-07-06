#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"
#include "Communicator.h"

RequestHandlerFactory::RequestHandlerFactory()
    : m_loginManager(),
    m_roomManager(RoomManager(this)),
    m_database(new SqliteDatabase()),
    m_statisticsManager(m_database),
    m_gameManager(m_database),
    m_communicatorPtr(new Communicator(*this))
{
    m_database->open();
}

RequestHandlerFactory::~RequestHandlerFactory() 
{ 
    delete m_database;
    delete m_communicatorPtr;
}

IRequestHandler* RequestHandlerFactory::createLoginRequestHandler(SOCKET sock)
{ return new LoginRequestHandler(*this, sock); }
IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user)
{ return new MenuRequestHandler(*this, user); }
IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room& room)
{ return new RoomAdminRequestHandler(*this, user, room); }
IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room& room)
{ return new RoomMemberRequestHandler(*this, user, room); }
IRequestHandler* RequestHandlerFactory::createGameRequestHandler(LoggedUser user, Room& room, RoomMember* preHandler)
{
    return new GameRequestHandler(*this, user, m_gameManager.createGame(room), preHandler);
}

LoginManager& RequestHandlerFactory::getLoginManager() { return m_loginManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return m_statisticsManager; }
RoomManager& RequestHandlerFactory::getRoomManager() { return m_roomManager; }
GameManager& RequestHandlerFactory::getGameManager() { return m_gameManager; }
Communicator& RequestHandlerFactory::getCommunicator() { return *m_communicatorPtr; }
