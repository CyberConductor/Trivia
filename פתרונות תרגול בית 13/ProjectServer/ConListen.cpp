#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include "json.hpp"
#include "SqliteDatabase.h"

using json = nlohmann::json;

// פונקציה עזר לקריאת 4 בייטים ולהמרתם ל-int (Big or Little endian תלוי)
int ReadIntFromBuffer(const char* buffer)
{
    // נניח little-endian (כמו ב-Windows)
    int val = 0;
    val |= (unsigned char)buffer[0];
    val |= ((unsigned char)buffer[1]) << 8;
    val |= ((unsigned char)buffer[2]) << 16;
    val |= ((unsigned char)buffer[3]) << 24;
    return val;
}

void HandleClient(SOCKET clientSocket)
{
    char messageCode;
    int messageSize;
    std::string jsonPayload;

    // 1. קריאת קוד ההודעה (1 בייט)
    int recvResult = recv(clientSocket, &messageCode, 1, 0);
    if (recvResult != 1) {
        std::cerr << "Error reading message code\n";
        return;
    }

    // 2. קריאת גודל ההודעה (4 בייטים)
    char sizeBuffer[4];
    recvResult = recv(clientSocket, sizeBuffer, 4, 0);
    if (recvResult != 4) {
        std::cerr << "Error reading message size\n";
        return;
    }
    messageSize = ReadIntFromBuffer(sizeBuffer);

    // 3. קריאת ההודעה עצמה (JSON) לפי הגודל שקיבלנו
    std::vector<char> buffer(messageSize);
    int totalReceived = 0;
    while (totalReceived < messageSize)
    {
        int bytesReceived = recv(clientSocket, buffer.data() + totalReceived, messageSize - totalReceived, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Error reading JSON payload\n";
            return;
        }
        totalReceived += bytesReceived;
    }
    jsonPayload.assign(buffer.begin(), buffer.end());

    // --- עכשיו נוכל לפרסר את ה JSON ולטפל לפי קוד ההודעה ---

    SqliteDatabase db;
    db.open();

    if (messageCode == 2) // SignUp example
    {
        try {
            auto parsed = json::parse(jsonPayload);
            std::string username = parsed.value("username", "");
            std::string password = parsed.value("password", "");
            std::string email = parsed.value("email", "");

            if (db.doesUserExist(username)) {
                std::string response = "User already exists";
                send(clientSocket, response.c_str(), (int)response.size(), 0);
            }
            else {
                int res = db.addNewUser(username, password, email);
                std::string response = (res == SQLITE_OK) ? "User added successfully" : "Error adding user";
                send(clientSocket, response.c_str(), (int)response.size(), 0);
            }
        }
        catch (const std::exception& e) {
            std::string response = std::string("JSON parsing error: ") + e.what();
            send(clientSocket, response.c_str(), (int)response.size(), 0);
        }
    }
    else
    {
        std::string response = "Unknown command";
        send(clientSocket, response.c_str(), (int)response.size(), 0);
    }
}
