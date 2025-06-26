#include "Room.h"
#include "IRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include <algorithm> 

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
    m_joinOrder.push_back(user.getUsername());
    currentUsers++;
}

void Room::removeUser(LoggedUser user)
{
    for (auto& pair : m_users)
    {
        if (pair.first.getUsername() == user.getUsername())
        {
            m_users.erase(user);
            m_joinOrder.erase(
                std::remove(m_joinOrder.begin(), m_joinOrder.end(), user.getUsername()),
                m_joinOrder.end()
            );

            if (--currentUsers == 0)
                m_metadata.status = false;
            return;
        }
    }
}

vector<string> Room::getAllUsers()
{
    return m_joinOrder;
}