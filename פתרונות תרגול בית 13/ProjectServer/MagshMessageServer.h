#pragma once

#include "MagshChat.h"
#include <deque>
#include <queue>
#include <map> 
#include <mutex>
#include <condition_variable>
#include <WinSock2.h>
#include "RecvMessage.h"
#include "LoginRequestHandler.h"


// Q: why do we need this class ?
// A: this is the main class which holds all the resources,
// accept new clients and handle them.
class MagshMessageServer
{
public:
	MagshMessageServer();
	~MagshMessageServer();
	void serve();


private:
	void bindAndListen();
	void acceptClient();
	void clientHandler(const SOCKET client_socket);
	void safeDeleteUser(const SOCKET id);

	void handleReceivedMessages();
	std::string getCurrentUser();
	std::string getNextUser();
	std::string get_user_name(const SOCKET id);
	void addReceivedMessage(RecvMessage*);
	static RecvMessage* build_receive_message(const SOCKET client_socket, const int msg_code);
	std::string getAllUsernames();
		
	SOCKET _socket;
	MagshChat _doc;

	// Queue for all clients. This way we will know who's the current writer.
	// SOCKET: client socket
	// string: userName
	std::deque<std::pair<SOCKET, std::string>> _clients;

	// Map for all the Login Requests
	// SOCKET: client socket
	// LoginRequestHandler: client login request
	std::map<SOCKET, LoginRequestHandler> _requests;

	// Queue for messages - Will hold the mssage code and the file data. To add messages use std::ref<const ClientSocket>
	// SOCKET: client socket
	// string: message
	std::queue<RecvMessage*> _messageHandler;

	std::mutex _mtxReceivedMessages;
	std::condition_variable _msgQueueCondition;
};

