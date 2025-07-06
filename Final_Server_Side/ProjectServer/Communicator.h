//#pragma comment(lib, "Ws2_32.lib")
#pragma once
#include <map>
#include <thread>
#include <exception>
#include "Helper.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

using std::map;
using std::exception;

class Communicator
{
public:
	Communicator(RequestHandlerFactory&);
	~Communicator();

	void startHandleRequests();
	void bindAndListen();
	void handleNewClient();
private:
	void handleClient(SOCKET);
<<<<<<< HEAD:Final_Server_Side/ProjectServer/Communicator.h
=======

>>>>>>> b2ee5add6bbab06d883a999d61c983eff6764292:פתרונות תרגול בית 13/ProjectServer/Communicator.h
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;

	friend RoomAdminRequestHandler;
};