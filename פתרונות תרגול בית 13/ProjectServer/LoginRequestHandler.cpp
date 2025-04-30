#include "LoginRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include <stdexcept>

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
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(requestInfo.buffer);
        SignupResponse signupResponse = { 1 }; // 1 = הצלחה
        result.response = JsonResponsePacketSerializer::serializeResponse(signupResponse);
        result.newHandler = this;
    }
    else
    {
        // למקרה שמישהו עוקף - זורקים חריגה
        throw std::invalid_argument("Invalid request code");
    }

    return result;
}
