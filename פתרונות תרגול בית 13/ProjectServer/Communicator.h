#pragma comment(lib, "Ws2_32.lib")
#pragma once
#include <map>
#include <thread>
#include <WinSock2.h>
#include <Windows.h>
#include "Helper.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

using std::map;

class Communicator
{
public:
	Communicator();
	~Communicator();

	//TODO: add return parameter
	void startHandleRequests();
	void bindAndListen();
	void handleNewClient();

private:
	void handleClient(SOCKET);

	
	map<SOCKET, IRequestHandler*> m_clients;
	SOCKET m_serverSocket;
};