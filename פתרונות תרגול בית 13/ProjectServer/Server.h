#pragma once

#include <deque>
#include <queue>
#include <map> 
#include <mutex>
#include <condition_variable>
#include "LoginRequestHandler.h"
#include "Communicator.h"

using std::thread;
using std::string;
using std::queue;
using std::deque;
using std::map;
using std::mutex;
using std::condition_variable;
using::std::pair;

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

