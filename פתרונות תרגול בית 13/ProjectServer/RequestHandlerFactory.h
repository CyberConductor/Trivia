#pragma once
#include <WinSock2.h>
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "SqliteDatabase.h"
#include "StatisticsManager.h"

//forward declaration to prevent circular dependency
class Communicator;
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory();
	~RequestHandlerFactory();

	//creators
	IRequestHandler* createLoginRequestHandler(SOCKET);
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
};