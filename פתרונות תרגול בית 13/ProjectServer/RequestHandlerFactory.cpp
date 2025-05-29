#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory()
{
	m_loginManager = LoginManager();
	m_roomManager = RoomManager();
	m_database = new SqliteDatabase();
	m_database->open();
	m_statisticsManager = StatisticsManager(m_database);
}

RequestHandlerFactory::~RequestHandlerFactory() { delete m_database; }

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler() { return new LoginRequestHandler(*this); }
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(LoggedUser user) 
{ return new MenuRequestHandler(*this, user); }

LoginManager& RequestHandlerFactory::getLoginManager() { return m_loginManager; }
StatisticsManager& RequestHandlerFactory::getStatisticsManager() { return m_statisticsManager; }
RoomManager& RequestHandlerFactory::getRoomManager() { return m_roomManager; }

