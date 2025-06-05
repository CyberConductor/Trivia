#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory()
    : m_loginManager(),
    m_roomManager(),
    m_database(new SqliteDatabase()),
    m_statisticsManager(m_database)
{
    m_database->open();
}

RequestHandlerFactory::~RequestHandlerFactory() { delete m_database; }

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() { return new LoginRequestHandler(*this); }
IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user) 
{ return new MenuRequestHandler(*this, user); }

LoginManager& RequestHandlerFactory::getLoginManager() { return m_loginManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return m_statisticsManager; }
RoomManager& RequestHandlerFactory::getRoomManager() { return m_roomManager; }