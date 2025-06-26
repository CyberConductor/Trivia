#include "Room.h"
#include "IRequestHandler.h"
#include "RoomAdminRequestHandler.h"

Room::Room(RoomData data) 
    : m_metadata(data)
{
    currentUsers = 0;
}

Room::~Room()
{
    for (auto& pair : m_users)
    {
        delete(pair.second);
    }
}

void Room::addUser(LoggedUser user, IRequestHandler* handler)
{
    if (currentUsers == m_metadata.maxPlayers)//check the max amount
        return;
    for (auto& pair : m_users)
        if (pair.first.getUsername() == user.getUsername())
            return;
    m_users.insert({ user, handler });
    currentUsers++;
}

void Room::removeUser(LoggedUser user)
{
    for (auto& pair : m_users)
    {
        if (pair.first.getUsername() == user.getUsername())
        {
            delete(pair.second);
            m_users.erase(user);
            if (--currentUsers == 0)
                m_metadata.status = false;
            return;
        }
    }
}

vector<string> Room::getAllUsers()
{
    vector<string> res;
    for (auto& user : m_users)
        res.push_back(user.first.getUsername());
    return res;
}