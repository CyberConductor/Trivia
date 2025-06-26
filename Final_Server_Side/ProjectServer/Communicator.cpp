#include "Communicator.h"

static const unsigned short PORT = 8826;
static const unsigned int IFACE = 0;

Communicator::Communicator(RequestHandlerFactory& factory) 
    : m_handlerFactory(factory)
{
    m_serverSocket = INVALID_SOCKET;
}

Communicator::~Communicator()
{
    // close the main server socket
    closesocket(m_serverSocket);

    // clean up all allocated request handlers
    for (auto& pair : m_clients)
    {
        delete pair.second; // free IRequestHandler*
        closesocket(pair.first); // close client socket
    }

    // clear the map
    m_clients.clear();
}

// listen to connecting requests from clients
// accept them, and create thread for each client
void Communicator::bindAndListen()
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_serverSocket == INVALID_SOCKET)
        throw exception(__FUNCTION__ " - socket");

    struct sockaddr_in sa = { 0 };
    sa.sin_port = htons(PORT);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = IFACE; // 0 = INADDR_ANY

    if (::bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
        throw exception(__FUNCTION__ " - bind");
    TRACE("binded");

    if (::listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
        throw exception(__FUNCTION__ " - listen");
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
    SOCKET clientSocket = accept(m_serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        throw exception(__FUNCTION__ " - accept");
    }
    TRACE("new client accepted");

    // create a thread for the new client
    std::thread clientThread([this, clientSocket]()
    {
        bool handlerAddedToMap = false;
        IRequestHandler* handler = nullptr;

        try
        {
            handler = m_handlerFactory.createLoginRequestHandler(clientSocket);
            RequestInfo requestInfo = Helper::getRequestInfo(clientSocket);
            RequestResult result = handler->handleRequest(requestInfo);

            auto buffer = result.response;
            int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
            std::string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
            json j = json::parse(jsonStr);
            LoginResponse loginResponse;
            loginResponse.status = j["status"];
            //check the client status
            if (loginResponse.status)

            {
                Helper::sendData(clientSocket, std::string(buffer.begin(), buffer.end()));
                {
                    m_clients[clientSocket] = result.newHandler;
                    handlerAddedToMap = true;
                }

                delete handler;
                std::thread([this, clientSocket]() {
                    handleClient(clientSocket);
                    }).detach();
            }
            else
            {
                Helper::sendData(clientSocket, std::string(buffer.begin(), buffer.end()));
            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Client " << clientSocket << " error: " << ex.what() << std::endl;

            if (handlerAddedToMap)
            {
                delete m_clients[clientSocket];
                m_clients.erase(clientSocket);
            }
            closesocket(clientSocket);
        }
    });
    clientThread.detach();// don't wait for the thread
}

void Communicator::handleClient(SOCKET sock)
{
    IRequestHandler* handler = m_clients[sock];

    try
    {
        while (true)
        {
            // receive and deserialize the request
            RequestInfo request = Helper::getRequestInfo(sock);
            // process the request through the current handler
            RequestResult result = handler->handleRequest(request);
            // send back the response
            Helper::sendData(sock, string(result.response.begin(), result.response.end()));
            // if the handler changed, replace and delete the old one
            if (result.newHandler != handler)
            {
                // check if the new handler is RoomAdminRequestHandler
                if (auto adminHandler = dynamic_cast<RoomAdminRequestHandler*>(handler))
                {
                    if (dynamic_cast<GameRequestHandler*>(result.newHandler))
                    {
                        for (auto& userPair : adminHandler->m_room.m_users)
                        {
                            const LoggedUser& user = userPair.first;
                            IRequestHandler*& userHandler = userPair.second;

                            // convert all the room members to game request handlers
                            if (adminHandler != userHandler)
                                if (auto memberHandler = dynamic_cast<RoomMember*>(userHandler))
                                    userPair.second = (IRequestHandler*)m_handlerFactory.createGameRequestHandler(user, adminHandler->m_room, memberHandler);
                        }
                        handler = result.newHandler;
                        m_clients[sock] = handler;
                    }
                }
                if (auto gameHandler = dynamic_cast<GameRequestHandler*>(handler))
                {
                    while (true)
                    {
                        // if leaveGame was called, break out of game mode
                        if (gameHandler != result.newHandler)
                        {
                            handler = result.newHandler;
                            m_clients[sock] = handler;
                            delete gameHandler;
                            break;
                        }

                        request = Helper::getRequestInfo(sock);
                        result = handler->handleRequest(request);
                        Helper::sendData(sock, string(result.response.begin(), result.response.end()));
                    }
                }
                else
                {
                    delete handler;
                    handler = result.newHandler;
                    m_clients[sock] = handler;
                }
            }
        }
    }
    catch (const exception& e)
    {
        std::cerr << "Client disconnected or error: " << e.what() << std::endl;

        delete handler;
        closesocket(sock);
        m_clients.erase(sock);
    }
}