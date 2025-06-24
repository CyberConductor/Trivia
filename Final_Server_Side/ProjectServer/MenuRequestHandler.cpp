#include "MenuRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include <stdexcept>

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, LoggedUser user) 
    : m_handlerFactory(factory),
    m_user(user) 
{}

bool MenuRequestHandler::isRequestRelevant(RequestInfo req)
{
    return (req.id >= Request_Signout && req.id <= Request_GetPersonalStats);
}

RequestResult MenuRequestHandler::handleRequest(RequestInfo req)
{
    if (!isRequestRelevant(req))
    {
        // irrelevant request
        return generateErrorResponse("Unrecognized request type", this);
    }
    switch (req.id)
    {
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
    string username = m_user.getUsername();
    try
    {
        m_handlerFactory.getLoginManager().logout(username);

        RequestResult result;
        result.response = JsonResponsePacketSerializer::serializeLogoutResponse({ 1 });
        result.newHandler = m_handlerFactory.createLoginRequestHandler(m_user.getSocket());
        return result;
    }
    catch(...)
    {
        IRequestHandler* handler = m_handlerFactory.createLoginRequestHandler(m_user.getSocket());
        return generateErrorResponse("failed to signout" + username, handler);
    }
}

RequestResult MenuRequestHandler::getRooms(RequestInfo requestInfo)
{
    vector<RoomData> rooms = m_handlerFactory.getRoomManager().getRooms();
    
    RequestResult result;
    result.response = JsonResponsePacketSerializer::serializeGetRoomsResponse({ 1, rooms });
    result.newHandler = this;
    return result;
}

RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo requestInfo)
{
    GetPlayersInRoomRequest req = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(requestInfo.buffer);

    try
    {
        Room& room = m_handlerFactory.getRoomManager().getRoom(req.roomId);
        vector<string> players = room.getAllUsers();
        return { JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse({ players }), this };
    }
    catch (const std::exception&)
    {
        return generateErrorResponse("No room found with the given ID", this);
    }
}

RequestResult MenuRequestHandler::getPersonalStats(RequestInfo requestInfo)
{
    string username = m_user.getUsername();
    if (!m_handlerFactory.getStatisticsManager().doesUserExist(username))
        return generateErrorResponse("user \"" + username + "\" not found in the database.", this);

    UserStatistics userstats = m_handlerFactory.getStatisticsManager().getUserStatistics(username);
    json statistics;
    statistics["username"] = username;
    statistics["averageAnswerTime"] = userstats.averageAnswerTime;
    statistics["totalAnswers"] = userstats.totalAnswers;
    statistics["correctAnswers"] = userstats.correctAnswers;
    statistics["gamesPlayed"] = userstats.gamesPlayed;
    statistics["totalScore"] = userstats.totalScore;

    return { JsonResponsePacketSerializer::serializegetPersonalStatsResponse({ 1, statistics }), this };
}

RequestResult MenuRequestHandler::getHighScore(RequestInfo requestInfo)
{
    map<string, int> stats = m_handlerFactory.getStatisticsManager().getHighScores();
    return { JsonResponsePacketSerializer::serializegetHighScoreResponse({ 1, stats }), this };
}

RequestResult MenuRequestHandler::joinRoom(RequestInfo requestInfo)
{
    JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(requestInfo.buffer);
    Room& room = m_handlerFactory.getRoomManager().getRoom(request.roomId);
    IRequestHandler* handler = m_handlerFactory.createRoomMemberRequestHandler(m_user, room);
    room.addUser(m_user, handler);

    return { JsonResponsePacketSerializer::serializeJoinRoomResponse({ 1 }), handler };
}

RequestResult MenuRequestHandler::createRoom(RequestInfo requestInfo)
{
    //deserialize request
    CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);
    int id = m_handlerFactory.getRoomManager().getFreeId();
    RoomData roomData = { id, request.roomName, request.maxUsers, request.questionCount, request.answerTimeOut, false };
    Room& room = m_handlerFactory.getRoomManager().createRoom(m_user, roomData);

    IRequestHandler* handler = m_handlerFactory.createRoomAdminRequestHandler(m_user, room);
    return { JsonResponsePacketSerializer::serializeCreateRoomResponse({ 1 , room.m_metadata.id }), handler };
}