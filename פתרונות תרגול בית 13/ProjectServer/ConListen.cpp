#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include "json.hpp"
#include "SqliteDatabase.h"
#include "StatisticsManager.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

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
    if (messageCode == Request_Signup) // Signup
    {
        try 
        {
           //30-33 maybe move here

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

    if (messageCode == Request_Login)
    {

    }
    if (messageCode == Request_JoinRoom)
    {

    }
    else if (messageCode == Request_GetPersonalStats) // Request_GetPersonalStats
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

    else
    {
        ErrorResponse err{ "Unknown command" };
        Buffer errBuf = JsonResponsePacketSerializer::serializeErrorResponse(err);
        send(clientSocket, reinterpret_cast<const char*>(errBuf.data()), (int)errBuf.size(), 0);
    }
}
