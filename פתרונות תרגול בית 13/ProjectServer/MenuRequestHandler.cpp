#include "MenuRequestHandler.h"
#include <stdexcept>

MenuRequestHandler::MenuRequestHandler() {}

bool MenuRequestHandler::isRequestRelevant(RequestInfo req)
{
    return (req.id >= Request_Signout || req.id <= Request_CreateRoom);
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo req)
{

    if (!isRequestRelevant(req))
    {
        // irrelevant requesst
        return generateErrorResponse("Unrecognized request type", this);
    }
    switch (req.id)
    {
    default:
        //shouldn't reach here
        return generateErrorResponse("Invalid request", this);
    }
}