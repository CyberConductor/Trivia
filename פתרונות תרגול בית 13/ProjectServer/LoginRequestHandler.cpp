#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include <stdexcept>

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory) 
    : m_handlerFactory(factory) {}

bool LoginRequestHandler::isRequestRelevant(RequestInfo requestInfo)
{
    // בודק אם הקוד מתאים ל-Login או Signup
    return (requestInfo.id == 1 || requestInfo.id == 2);
}

RequestResult LoginRequestHandler::handleRequest(RequestInfo requestInfo)
{
    RequestResult result;

    if (!isRequestRelevant(requestInfo))
    {
        // בקשה לא רלוונטית -> מחזיר שגיאה
        ErrorResponse errorResponse = { "Invalid request" };
        result.response = JsonResponsePacketSerializer::serializeResponse(errorResponse);
        result.newHandler = this;
        return result;
    }

    if (requestInfo.id == 1) // login
    {
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
        // כאן אפשר לבדוק שם משתמש וסיסמה אמיתיים
        LoginResponse loginResponse = { 1 }; // 1 = הצלחה
        result.response = JsonResponsePacketSerializer::serializeResponse(loginResponse);
        result.newHandler = this;
    }
    else if (requestInfo.id == 2) // signup
    {
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
        SignupResponse signupResponse = { 1 }; // 1 = הצלחה
        result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
        result.newHandler = this;
    }
    else
    {
        ErrorResponse err = { "Invalid request code" };
        result.response = JsonResponsePacketSerializer::serializeResponse(err);
        result.newHandler = this;
    }

    return result;
}

RequestResult LoginRequestHandler::login(RequestInfo requestInfo)
{
    if (requestInfo.id == 1)
    {
        LoginManager manager = m_handlerFactory.getLoginManager();
        LoginRequest req = JsonRequestPacketDeserializer::deserializeLoginRequest(requestInfo.buffer);
        bool success = manager.login(req.username, req.password);

        LoginResponse loginResp = { success ? 1 : 0 };
        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeResponse(loginResp);
        result.newHandler = success ? m_handlerFactory.createLoginRequestHandler() : this;

        return result;
    }

    //else - generate error response
    ErrorResponse err = { "Invalid login request" };

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeResponse(err);
    result.newHandler = this;

    return result;
}

RequestResult LoginRequestHandler::signup(RequestInfo requestInfo)
{
    if (requestInfo.id == 2)
    {
        LoginManager manager = m_handlerFactory.getLoginManager();
        SignupRequest req = JsonRequestPacketDeserializer::deserializeSignupRequest(requestInfo.buffer);
        bool success = manager.signup(req.username, req.password, req.email);

        SignupResponse signupResp = { success ? 1 : 0 };
        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeResponse(signupResp);
        result.newHandler = success ? m_handlerFactory.createLoginRequestHandler() : this;

        return result;
    }

    //else - generate error response
    ErrorResponse err = { "Invalid signup request" };

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeResponse(err);
    result.newHandler = this;

    return result;
}