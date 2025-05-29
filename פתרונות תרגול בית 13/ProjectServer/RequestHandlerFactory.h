#pragma once
#include "LoginManager.h"
#include "SqliteDatabase.h"
#include "RoomManager.h"
#include "StatisticsManager.h"

//forward declaration to prevent circular dependency 
class LoginRequestHandler;
class MenuRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory();
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();
	MenuRequestHandler* createMenuRequestHandler(LoggedUser);
	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
private:
	LoginManager m_loginManager;
	SqliteDatabase* m_database;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
};