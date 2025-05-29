#include "Server.h"
#include <exception>
#include <iostream>
#include <string>
#include <numeric>

using std::unique_lock;
using std::vector;
using std::exception;

Server::Server()
{
	// notice that we step out to the global namespace
	// for the resolution of the function socket
	_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
		throw exception( __FUNCTION__ " - socket");
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
	while (true)
	{
		thread(&Communicator::startHandleRequests, &m_communicator).detach();

		//// the main thread is only accepting clients 
		//// and add then to the list of handlers
		//TRACE("accepting client...");
		//acceptClient();
	}
}