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
    bindAndListen(); // first, bind and listen on the port

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
                // here you should handle the client requests
                IRequestHandler* handler = nullptr; // later: create handler from a factory

                this->m_clients[clientSocket] = handler;

                bool isActive = true;
                while (isActive)
                {
                    int messageCode = Helper::getMessageTypeCode(clientSocket);

                    if (messageCode == 0)
                    {
                        // client disconnected
                        TRACE("client disconnected");
                        isActive = false;
                        break;
                    }

                    IRequestHandler* currentHandler = this->m_clients[clientSocket];
                    if (currentHandler == nullptr)
                    {
                        TRACE("no handler for client");
                        isActive = false;
                        break;
                    }

                    // now you would normally call currentHandler->handleRequest() etc.
                    // but since no RequestHandler implemented yet, we just print the code
                    TRACE("received message code: %d", messageCode);

                    // todo: handle the request properly with handler
                }

                // clean up
                closesocket(clientSocket);
                this->m_clients.erase(clientSocket);
                TRACE("client socket closed");
            }
            catch (std::exception& e)
            {
                TRACE("error in client thread: %s", e.what());
                closesocket(clientSocket);
                this->m_clients.erase(clientSocket);
            }
        });

    clientThread.detach(); // don't wait for the thread
}
