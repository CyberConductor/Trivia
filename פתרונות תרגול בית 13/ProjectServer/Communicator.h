//#pragma comment(lib, "Ws2_32.lib")
#pragma once
#include <map>
#include <thread>
#include <exception>
//#include <WinSock2.h>
//#include <Windows.h>
#include "Helper.h"
#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

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
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;
};