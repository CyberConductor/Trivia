#include "LoginRequestHandler.h"
#include <stdexcept>

<<<<<<< HEAD:Final_Server_Side/ProjectServer/LoginRequestHandler.cpp
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory, SOCKET sock) 
    : m_handlerFactory(factory),
    m_loginManager(factory.getLoginManager()),
    m_socket(sock)
=======
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) 
    : m_handlerFactory(factory),
    m_loginManager(factory.getLoginManager())
>>>>>>> b2ee5add6bbab06d883a999d61c983eff6764292:פתרונות תרגול בית 13/ProjectServer/LoginRequestHandler.cpp
{}

bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
    // בודק אם הקוד מתאים ל-Login או Signup
    return (requestInfo.id == Request_Login || requestInfo.id == Request_Signup);
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo requestInfo)
{
    if (!isRequestRelevant(requestInfo))
    {
        // בקשה לא רלוונטית -> מחזיר שגיאה
        return generateErrorResponse("Unrecognized request type", this);
    } 
    switch (requestInfo.id)
    {
    case Request_Login:
        return login(requestInfo);
    case Request_Signup:
        return signup(requestInfo);
    default:
        //shouldn't reach here
        return generateErrorResponse("Invalid request", this);
    }
}

RequestResult LoginRequestHandler::login(RequestInfo requestInfo)
{
    LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
    unsigned int success = m_loginManager.login(req.username, req.password, m_socket);

    Buffer buffer = JsonResponsePacketSerializer::serializeLoginResponse({ success });
<<<<<<< HEAD:Final_Server_Side/ProjectServer/LoginRequestHandler.cpp
    return { buffer, success ? m_handlerFactory.createMenuRequestHandler({ req.username, m_socket }) : this};
=======
    return { buffer, success ? m_handlerFactory.createMenuRequestHandler() : this};
>>>>>>> b2ee5add6bbab06d883a999d61c983eff6764292:פתרונות תרגול בית 13/ProjectServer/LoginRequestHandler.cpp
}

RequestResult LoginRequestHandler::signup(RequestInfo requestInfo)
{
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
    unsigned int success = m_loginManager.signup(req.username, req.password, req.email, m_socket);

    Buffer buffer = JsonResponsePacketSerializer::serializeSignupResponse({ success });
<<<<<<< HEAD:Final_Server_Side/ProjectServer/LoginRequestHandler.cpp
    return { buffer, success ? m_handlerFactory.createMenuRequestHandler({ req.username, m_socket }) : this };
=======
    return { buffer, success ? m_handlerFactory.createMenuRequestHandler() : this };
>>>>>>> b2ee5add6bbab06d883a999d61c983eff6764292:פתרונות תרגול בית 13/ProjectServer/LoginRequestHandler.cpp
}