#include "Server.h"
#include <exception>
#include <iostream>
#include <string>
#include <numeric>

// using static const instead of macros 
static const unsigned short PORT = 8826;
static const unsigned int IFACE = 0;

using std::string;
using std::mutex;
using std::unique_lock;
using std::vector;


Server::Server()
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket
	_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
		throw std::exception( __FUNCTION__ " - socket");
}

Server::~Server()
{
	TRACE(__FUNCTION__ " closing accepting socket");
	// why is this try necessarily ?
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		::closesocket(_socket);
	}
	catch (...) {}
}

void Server::run()
{	
	thread(&Communicator::startHandleRequests, &m_communicator).detach();

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		TRACE("accepting client...");
		acceptClient();
	}
}


// listen to connecting requests from clients
// accept them, and create thread for each client
void Server::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;
	// again stepping out to the global namespace
	if (::bind(_socket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	TRACE("binded");

	if (::listen(_socket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	TRACE("listening...");
}

void Server::acceptClient()
{
	SOCKET client_socket = accept(_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	TRACE("Client accepted !");

	//Send welcome massage
	const string hello = "Hello";
	Helper::sendData(client_socket, hello);

	// create new thread for client	and detach from it
	std::thread tr(&Server::clientHandler, this, client_socket);
	tr.detach();

}



void Server::clientHandler(const SOCKET client_socket)
{
	RecvMessage* currRcvMsg = nullptr;
	try
	{		
		// get the first message code
 		int msgCode = Helper::getMessageTypeCode(client_socket);

		while (msgCode != 0 && (msgCode == MT_CLIENT_LOG_IN || msgCode == MT_CLIENT_UPDATE || msgCode == MT_CLIENT_FINISH))
		{
			currRcvMsg = build_receive_message(client_socket, msgCode);
			addReceivedMessage(currRcvMsg);

			msgCode = Helper::getMessageTypeCode(client_socket);			
		}

		currRcvMsg = build_receive_message(client_socket, MT_CLIENT_EXIT);
		addReceivedMessage(currRcvMsg);

	}
	catch (const std::exception& e)
	{
		std::cout << "Exception was catch in function clientHandler. socket=" << client_socket << ", what=" << e.what() << std::endl;
		currRcvMsg = build_receive_message(client_socket, MT_CLIENT_EXIT);
		addReceivedMessage(currRcvMsg);
	}
	closesocket(client_socket);
}

void Server::addReceivedMessage(RecvMessage* msg)
{
	unique_lock<mutex> lck(_mtxReceivedMessages);

	_messageHandler.push(msg);
	lck.unlock();
	_msgQueueCondition.notify_all();
	
}

RecvMessage* Server::build_receive_message(const SOCKET client_socket, const int msg_code)
{
	RecvMessage* msg = nullptr;
	vector<string> values;
	if (msg_code == MT_CLIENT_LOG_IN)
	{
		int userSize = Helper::getIntPartFromSocket(client_socket, 2);
		string userName = Helper::getStringPartFromSocket(client_socket, userSize);
		values.push_back(userName);
	}
	else if (msg_code == MT_CLIENT_UPDATE)
	{
		// Get second end point user name size, and get username
		const int second_user_size = Helper::getIntPartFromSocket(client_socket, 2);
		const string user_name = Helper::getStringPartFromSocket(client_socket, second_user_size);
		values.push_back(user_name);
		
		// Get message size and get message
		const int file_size = Helper::getIntPartFromSocket(client_socket, 5);
		const string file_content = Helper::getStringPartFromSocket(client_socket, file_size);
		values.push_back(file_content);
	}

	msg = new RecvMessage(client_socket, msg_code, values);
	return msg;

}


// remove the user from queue
void Server::safeDeleteUser(const SOCKET id)
{
	try
	{
		for (unsigned int i = 0; i < _clients.size(); i++)
		{
			if (_clients[i].first == id)
			{
				TRACE("REMOVED %d, %s from clients list", id, _clients[i].second.c_str());
				_clients.erase(_clients.begin() + i);
				
			}
		}
	}
	catch (...) {}
	
}

void Server::handleReceivedMessages()
{
	int msgCode = 0;
	SOCKET clientSock = 0;
	string userName;
	while (true)
	{
		try
		{
			unique_lock<mutex> lck(_mtxReceivedMessages);

			// Wait for clients to enter the queue.
			if (_messageHandler.empty())
				_msgQueueCondition.wait(lck);

			// in case the queue is empty.
			if (_messageHandler.empty())
				continue;

			RecvMessage* currMessage = _messageHandler.front();
			_messageHandler.pop();
			lck.unlock();

			// Extract the data from the tuple.
			clientSock = currMessage->getSock();
			msgCode = currMessage->getMessageCode();

			if (msgCode == MT_CLIENT_LOG_IN)
			{
				userName = currMessage->getValues()[0];
				_clients.emplace_back(clientSock, userName);

				TRACE("ADDED new client %d, %s to clients list", clientSock, userName.c_str());

				// After login the data in empty.
				Helper::send_update_message_to_client(clientSock, "", "", getAllUsernames());
			}

			else if (msgCode == MT_CLIENT_UPDATE)
			{
				//TRACE("Recieved update message from current client");
				string current_username = get_user_name(currMessage->getSock());
				string second_username = currMessage->getValues()[0];
				string new_message = currMessage->getValues()[1];
				if (!new_message.empty()) {
					_doc.write(current_username, second_username, new_message);
				}
				string conversation;
				if (!second_username.empty()) {
					conversation = _doc.read(current_username, second_username);
				}
				auto all_user_names = getAllUsernames();
				//std::cout << "All users: " << all_user_names << std::endl;
				Helper::send_update_message_to_client(clientSock, conversation, second_username, all_user_names);
			}
			else if (msgCode == MT_CLIENT_EXIT)
			{
				TRACE("Recieved exit message from client");
				safeDeleteUser(clientSock);
			}

			delete currMessage;
		}
		catch (...)
		{

		}
	}
}


std::string Server::getAllUsernames()
{
	const std::string delimiter = "&";
	return std::accumulate(std::begin(_clients), std::end(_clients), std::string(),
		[delimiter](const string& lhs, const std::pair<SOCKET, string>& rhs)
		{
			if (lhs.empty()) return rhs.second;
			else
				return lhs + delimiter + rhs.second; });
}


// get current user name (the writer)
std::string Server::getCurrentUser()
{

	if (_clients.size() < 1)
		return "";

	return _clients[0].second;
}

// get next user in queue
std::string Server::getNextUser()
{
	if (_clients.size() < 2)
		return "";

	return _clients[1].second;
}

std::string Server::get_user_name(const SOCKET id)
{
	for (const std::pair<SOCKET, string>& elem : _clients)
	{
		if (elem.first == id)
		{
			return elem.second;
		}		
	}
	return {};
}
