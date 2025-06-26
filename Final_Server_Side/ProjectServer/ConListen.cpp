#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include "json.hpp"
#include "SqliteDatabase.h"
#include "StatisticsManager.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
#include "Room.h"
#include "RoomManager.h"
#include "LoginManager.h"
using json = nlohmann::json;

typedef std::vector<unsigned char> Buffer;

void HandleClient(SOCKET clientSocket)
{
    Buffer buffer(1024);
    int bytesRead = recv(clientSocket, reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

    if (bytesRead < 5) {
        std::cerr << "Invalid message received\n";
        return;
    }

    buffer.resize(bytesRead);
    unsigned char messageCode = buffer[0];

    SqliteDatabase db;
    db.open();
   
    auto parsed = json::parse(std::string(buffer.begin() + 5, buffer.end()));
    std::string username = parsed.value("username", "");
    std::string password = parsed.value("password", "");
    std::string email = parsed.value("email", "");
    if (messageCode == Request_Login)
    {
        try
        {
            LoginManager loginManager(&db);
            LoginRequest request = JsonRequestPacketDeserializer::deserializeLoginRequest(buffer);

            std::cout << "[LOGIN] Username: " << request.username << ", Password: " << request.password << std::endl;

            if (!loginManager.login(request.username, request.password))
            {
                ErrorResponse err{ "Login failed: wrong credentials or user already logged in" };
                Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
                send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
                return;
            }

        
            LoginResponse response;
            response.status = 1;

            Buffer outBuffer = JsonResponsePacketSerializer::serializeLoginResponse(response);
            send(clientSocket, reinterpret_cast<const char*>(outBuffer.data()), (int)outBuffer.size(), 0);
        }
        catch (const std::exception& e)
        {
            ErrorResponse err{ std::string("Login error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
        }
    }
    
    else if (messageCode == Request_Signup) // Signup
    {
        try 
        {
            if (db.doesUserExist(username)) 
            {
                std::string response = "User already exists";
                send(clientSocket, response.c_str(), (int)response.size(), 0);
            }
            else 
            {
                int res = db.addNewUser(username, password, email);
                std::string response = (res == SQLITE_OK) ? "User added successfully" : "Error adding user";
                send(clientSocket, response.c_str(), (int)response.size(), 0);
            }
        }
        catch (const std::exception& e) {
            // std::string response = std::string("JSON parsing error: ") + e.what();
            // send(clientSocket, response.c_str(), (int)response.size(), 0);
        }
    }

    else if (messageCode == Request_Signout)
    {
        try
        {
            LoginManager loginManager(&db);

            std::cout << "[SIGNOUT] Username: " << username << std::endl;

            loginManager.logout(username);

            json responseJson = {
                {"status", 1},
                {"message", "User signed out successfully"}
            };

            std::string responseStr = responseJson.dump();
            send(clientSocket, responseStr.c_str(), (int)responseStr.size(), 0);
        }
        catch (const std::exception& e)
        {
            ErrorResponse err{ std::string("Signout error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
        }
    }
    else if (messageCode == Request_GetRooms)
    {
        try
        {
            extern RoomManager g_roomManager; 

            std::vector<RoomData> roomList = g_roomManager.getRooms();

            GetRoomsResponse response;
            response.status = 1;
            response.rooms = roomList;

            Buffer outBuffer = JsonResponsePacketSerializer::serializeGetRoomsResponse(response);
            send(clientSocket, reinterpret_cast<const char*>(outBuffer.data()), static_cast<int>(outBuffer.size()), 0);
        }
        catch (const std::exception& e)
        {
            ErrorResponse err{ std::string("GetRooms error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), static_cast<int>(errBuf.size()), 0);
        }
    }
    else if (messageCode == Request_GetPlayersInRoom)
    {
        try
        {
            GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(buffer);
            std::vector<std::string> players = db.getPlayersInRoom(request.roomId);
            GetPlayersInRoomResponse response;
            response.players = players;

            Buffer outBuffer = JsonResponsePacketSerializer::serializeGetPlayersInRoomResponse(response);

            send(clientSocket, reinterpret_cast<const char*>(outBuffer.data()), (int)outBuffer.size(), 0);
        }
        catch (const std::exception& e)
        {
            ErrorResponse err{ std::string("Error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
        }
    }

    else if (messageCode == Request_GetPersonalStats)
    {
        try {
            // Deserialize using existing function
            //auto request = JsonRequestPacketDeserializer::deserializeGetPersonalStatsRequest(buffer);
            //std::string username = request.username;

            StatisticsManager statsManager(&db);
            if (!statsManager.doesUserExist(username)) {
                ErrorResponse err{ "User does not exist" };
                Buffer errorBuffer = JsonResponsePacketSerializer::serializeErrorResponse(err);
                send(clientSocket, reinterpret_cast<const char*>(errorBuffer.data()), (int)errorBuffer.size(), 0);
                return;
            }

            UserStatistics stats = statsManager.getUserStatistics(username);

            getPersonalStatsResponse response;
            response.status = 1;
            response.statistics = {
                {"username", stats.username},
                {"averageAnswerTime", stats.averageAnswerTime},
                {"totalAnswers", stats.totalAnswers},
                {"correctAnswers", stats.correctAnswers},
                {"gamesPlayed", stats.gamesPlayed},
                {"totalScore", stats.totalScore}
            };

            Buffer outBuffer = JsonResponsePacketSerializer::serializegetPersonalStatsResponse(response);
            send(clientSocket, reinterpret_cast<const char*>(outBuffer.data()), (int)outBuffer.size(), 0);
        }
        catch (const std::exception& e) {
            ErrorResponse err{ std::string("Stats error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
        }

    }
    else if (messageCode == Request_GetHighScore)
    {
        try
        {
            StatisticsManager statsManager(&db);
            std::vector<std::string> topPlayers = statsManager.getHighScores();

            getHighScoreResponse response;
            response.status = 1;

            json playersArray = json::array();
            for (const auto& player : topPlayers)
            {
                playersArray.push_back(player);
            }
            response.statistics = playersArray;

            Buffer outBuffer = JsonResponsePacketSerializer::serializegetHighScoreResponse(response);
            send(clientSocket, reinterpret_cast<const char*>(outBuffer.data()), (int)outBuffer.size(), 0);
        }
        catch (const std::exception& e)
        {
            ErrorResponse err{ std::string("HighScore error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
        }
        }
    else if (messageCode == Request_CreateRoom)
    {
        try
        {
            //deserialize request from buffer
            CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(buffer);

            std::cout << "[CREATE ROOM] Room Name: " << request.roomName
                << ", Max Users: " << request.maxUsers
                << ", Question Count: " << request.questionCount
                << ", Timeout: " << request.answerTimeOut << std::endl;

            ////


            CreateRoomResponse response;
            response.status = 1; //success

            //serialize & send response
            Buffer outBuffer = JsonResponsePacketSerializer::serializeCreateRoomResponse(response);
            send(clientSocket, reinterpret_cast<const char*>(outBuffer.data()), (int)outBuffer.size(), 0);
        }
        catch (const std::exception& e)
        {
            ErrorResponse err{ std::string("CreateRoom error: ") + e.what() };
            Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
            send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
        }
        }
    else if (Request_JoinRoom)
    {

    }

    else
    {
        ErrorResponse err{ "Unknown command" };
        Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
        send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
    }
}
