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
        try
        {
            //craete new login handler
            m_clients.emplace(clientSocket, m_handlerFactory.createLoginRequestHandler(clientSocket));

            // get the client's request
            RequestInfo requestInfo = Helper::getRequestInfo(clientSocket);

            // process the request
            LoginRequestHandler handler = LoginRequestHandler(m_handlerFactory, clientSocket);
            RequestResult result = handler.handleRequest(requestInfo);

            //deserialize the response
            Buffer buffer = result.response;
            int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
            string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
            json j = json::parse(jsonStr);
            LoginResponse loginResponse;
            loginResponse.status = j["status"];
            
            //check the client status
            if (loginResponse.status == Response_Login)
            {
                delete m_clients[clientSocket];
                LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
                {
                    m_clients[clientSocket] = m_handlerFactory.createMenuRequestHandler({ req.username, clientSocket });
                }
            }

            // send back the response
            Helper::sendData(clientSocket, string(result.response.begin(), result.response.end()));
        }
        catch (const exception& ex)
        {
            ErrorResponse errorResponse = { ex.what() };
            Buffer errorBuffer = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
            Helper::sendData(clientSocket, string(errorBuffer.begin(), errorBuffer.end()));
            delete m_clients[clientSocket];
            m_clients[clientSocket] = nullptr;
        }
    });
    clientThread.detach();// don't wait for the thread
}