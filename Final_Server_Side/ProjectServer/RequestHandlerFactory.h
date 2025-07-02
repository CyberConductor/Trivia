#pragma once
#include <WinSock2.h>
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "GameManager.h"
#include "SqliteDatabase.h"
#include "StatisticsManager.h"

//forward declaration to prevent circular dependency
class Communicator;
class LoginRequestHandler;
class MenuRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class RoomMember;
class GameRequestHandler;

class RequestHandlerFactory
{
public:
	RequestHandlerFactory();
	~RequestHandlerFactory();

	//creators
	IRequestHandler* createLoginRequestHandler(SOCKET);
	IRequestHandler* createMenuRequestHandler(LoggedUser);
	IRequestHandler* createRoomAdminRequestHandler(LoggedUser, Room&);
	IRequestHandler* createRoomMemberRequestHandler(LoggedUser, Room&);
	IRequestHandler* createGameRequestHandler(LoggedUser, Room&, RoomMember*);

	//getters
	LoginManager& getLoginManager();
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();
	GameManager& getGameManager();
	Communicator& getCommunicator();

private:
	SqliteDatabase* m_database;
	StatisticsManager m_statisticsManager;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	GameManager m_gameManager;
	Communicator* m_communicatorPtr;
};