#pragma once
#include <exception>
#include "LoginRequestHandler.h"
#include "Communicator.h"

// Q: why do we need this class ?
// A: this is the main class which holds all the resources,
// accept new clients and handle them.
class Server
{
public:
	Server();
	~Server();
	void run();

private:
	SqliteDatabase m_database;
	Communicator m_communicator;
	RequestHandlerFactory m_handlerFactory;
};

