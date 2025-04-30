#include "Communicator.h"

// copied from Server.cpp
static const unsigned short PORT = 8826;
static const unsigned int IFACE = 0;

// listen to connecting requests from clients
// accept them, and create thread for each client
void Communicator::bindAndListen()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
        throw std::exception(__FUNCTION__ " - socket");

    struct sockaddr_in sa = { 0 };
    sa.sin_port = htons(PORT);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = IFACE; // 0 = INADDR_ANY

    if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
        throw std::exception(__FUNCTION__ " - bind");
    TRACE("binded");

    if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::exception(__FUNCTION__ " - listen");
    TRACE("listening...");
}


void Communicator::startHandleRequests()
{
    bindAndListen();

    while (true)
    {
        TRACE("waiting for new client...");
        handleNewClient();
    }
}


void Communicator::handleNewClient()
{

    SOCKET clientSocket = ::accept(m_serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        throw std::exception(__FUNCTION__ " - accept");
    }
    TRACE("new client accepted");

    // create a thread for the new client
    std::thread clientThread([this, clientSocket]()
    {
        try
        {
            // get the client's request
            RequestInfo requestInfo = Helper::getRequestInfo(clientSocket);

            // create the first handler - login/signup stage
            LoginRequestHandler handler;

            // check if the request is relevant
            if (!handler.isRequestRelevant(requestInfo))
            {
                ErrorResponse errorResponse = { "Unrecognized request type" };
                Buffer errorBuffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);
                Helper::sendData(clientSocket, string(errorBuffer.begin(), errorBuffer.end()));
                return;
            }

            // process the request
            RequestResult result = handler.handleRequest(requestInfo);

            // send back the response
            Helper::sendData(clientSocket, string(result.response.begin(), result.response.end()));
        }
        catch (const std::exception& ex)
        {
            ErrorResponse errorResponse = { ex.what() };
            Buffer errorBuffer = JsonResponsePacketSerializer::serializeResponse(errorResponse);
            Helper::sendData(clientSocket, string(errorBuffer.begin(), errorBuffer.end()));
        }
    });
    clientThread.detach();// don't wait for the thread
}