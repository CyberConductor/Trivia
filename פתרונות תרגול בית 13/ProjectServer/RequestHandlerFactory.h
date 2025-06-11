#pragma once
#include "LoginManager.h"
#include "RoomManager.h"
#include "SqliteDatabase.h"
#include "StatisticsManager.h"
#include "IRequestHandler.h"

//forward declaration to prevent circular dependency
class Communicator;
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(Communicator*);
	~RequestHandlerFactory();

	//creators
	LoginRequestHandler* createLoginRequestHandler();
	IRequestHandler* createMenuRequestHandler(LoggedUser);
	RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser, Room);
	RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser, Room);

	//getters
	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();

private:
	LoginManager m_loginManager;
	SqliteDatabase* m_database;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
	Communicator* m_communicator;
};