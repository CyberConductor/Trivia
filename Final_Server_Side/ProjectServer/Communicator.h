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
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;

	friend RoomAdminRequestHandler;
};