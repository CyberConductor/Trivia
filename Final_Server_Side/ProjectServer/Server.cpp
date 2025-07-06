#include "Server.h"
#include <iostream>

Server::Server()
    : m_database(),
    m_handlerFactory()
{}

Server::~Server()
{
    TRACE(__FUNCTION__ " shutting down server");
    // no need to manually delete anything
}

void Server::run()
{
    TRACE("starting server...");
    try
    {
        m_handlerFactory.getCommunicator().startHandleRequests();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}