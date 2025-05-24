#pragma once

#include <deque>
#include <queue>
#include <map> 
#include <mutex>
#include <condition_variable>
#include "Chat.h"
#include "RecvMessage.h"
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
	void bindAndListen();
	void acceptClient();
	void clientHandler(const SOCKET client_socket);
	void safeDeleteUser(const SOCKET id);

	void handleReceivedMessages();
	string getCurrentUser();
	string getNextUser();
	string get_user_name(const SOCKET id);
	void addReceivedMessage(RecvMessage*);
	static RecvMessage* build_receive_message(const SOCKET client_socket, const int msg_code);
	string getAllUsernames();
		
	SOCKET _socket;
	Chat _doc;

	// Queue for all clients. This way we will know who's the current writer.
		// SOCKET: client socket
		// string: userName
	deque<pair<SOCKET, string>> _clients;


	// Queue for messages - Will hold the mssage code and the file data. To add messages use std::ref<const ClientSocket>
	// SOCKET: client socket
	// string: message
	queue<RecvMessage*> _messageHandler;

	mutex _mtxReceivedMessages;
	condition_variable _msgQueueCondition;

	//TODO: create HandlerFactory
	//IDatabase m_database;
	Communicator m_communicator;
	//RequestHandlerFactory m_handlerFactory;
};

