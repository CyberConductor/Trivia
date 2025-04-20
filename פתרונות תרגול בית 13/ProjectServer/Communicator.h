#pragma once

#include <map>
#include <Windows.h>
#include "IRequestHandler.h"

using std::map;

class Communicator
{
public:
	//TODO: add return parameter
	startHandleRequests();
	bindAndListen();
	handleNewClient();

private:
	//TODO: create handler factory

	// Map for all the Login Requests
	// SOCKET: client socket
	// LoginRequestHandler: client login request
	map<SOCKET, IRequestHandler*> m_clients;

	RequestHandlerFactory& m_handlerFactory; 
	SOCKET m_serverSocket;
};