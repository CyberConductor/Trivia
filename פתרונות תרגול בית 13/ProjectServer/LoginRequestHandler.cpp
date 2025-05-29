#include "LoginRequestHandler.h"
#include <stdexcept>

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) 
    : m_handlerFactory(factory), m_loginManager(factory.getLoginManager()) {}

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
    unsigned int success = m_loginManager.login(req.username, req.password);

    LoginResponse loginResp = { success };
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeLoginResponse(loginResp);
    if(success)
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
    result.newHandler = this;

    return result;
}

RequestResult LoginRequestHandler::signup(RequestInfo requestInfo)
{
    SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
    unsigned int success = m_loginManager.signup(req.username, req.password, req.email);

    SignupResponse signupResp = { success };
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeSignupResponse(signupResp);
    if(success)
        result.newHandler = m_handlerFactory.createLoginRequestHandler();
    result.newHandler = this;

    return result;
}