#include "MenuRequestHandler.h"
#include <stdexcept>

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, LoggedUser& user) 
    : m_handlerFactory(factory), m_user(user) {}

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
    case Request_Login:
        return signout(req);
    case Request_Signup:
        return getRooms(req);
    case Request_Signout:
        return signout(req);
    case Request_GetRooms:
        return getRooms(req);
    case Request_GetPlayersInRoom:
        return getPlayersInRoom(req);
    case Request_GetPersonalStats:
        return getPersonalStats(req);
    case Request_GetHighScore:
        return getHighScore(req);
    case Request_JoinRoom:
        return joinRoom(req);
    case Request_CreateRoom:
        return createRoom(req);

    default:
        //shouldn't reach here
        return generateErrorResponse("Invalid request", this);
    }
}

RequestResult MenuRequestHandler::signout(RequestInfo requestInfo)
{
    //deserialize request
    Buffer buffer = requestInfo.buffer;
    int jsonSize = (buffer[1] << 24) | (buffer[2] << 16) | (buffer[3] << 8) | buffer[4];
    string jsonStr(buffer.begin() + 5, buffer.begin() + 5 + jsonSize);
    json j = json::parse(jsonStr);
    string username = j["username"];
    try
    {
        m_handlerFactory.getLoginManager().logout(username);

        LogoutResponse resp = { 1 };
        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeLogoutResponse(resp);
        result.newHandler = m_handlerFactory.createMenuRequestHandler(LoggedUser(username));

        return result;
    }
    catch(...)
    {
        LogoutResponse resp = { 0 };
        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeLogoutResponse(resp);
        result.newHandler = this;

        return result;
    }
}

RequestResult MenuRequestHandler::getRooms(RequestInfo requestInfo)
{
    vector<RoomData> rooms = m_handlerFactory.getRoomManager().getRooms();
    GetRoomsResponse resp = { 1, rooms };

    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetRoomsResponse(resp);
    result.newHandler = this;

    return result;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo requestInfo)
{
    GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(requestInfo.buffer);
    Room* room = m_handlerFactory.getRoomManager().getRoom(req.roomId);
    if (room)
    {
        vector<string> players = room->getAllUsers();
        GetPlayersInRoomResponse resp = { players };
        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(resp);
        result.newHandler = this;

        return result;
    }

    return generateErrorResponse("No room found with the given ID", this);
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo requestInfo)
{
    
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo requestInfo)
{
    return RequestResult();
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo requestInfo)
{
    return RequestResult();
}

RequestResult MenuRequestHandler::createRoom(RequestInfo requestInfo)
{
    return RequestResult();
}