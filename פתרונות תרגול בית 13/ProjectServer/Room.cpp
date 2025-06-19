#include "Room.h"

Room::Room(RoomData data) : m_metadata(data)
{
    currentUsers = 0;
}

void Room::addUser(LoggedUser user)
{
    if (currentUsers == m_metadata.maxPlayers)//check the max amount
        return;
	for (auto& it : m_users)
		if (it.getUsername() == user.getUsername())//check that there is no another player with the same username
			return;

	m_users.push_back(user);
    currentUsers++;
}

void Room::removeUser(LoggedUser user)
{
    for (auto it = m_users.begin(); it != m_users.end(); ++it)
    {
        if (it->getUsername() == user.getUsername())
        {
            m_users.erase(it);
            if (--currentUsers == 0)
                m_metadata.status = false;
            return;
        }
    }
}

vector<string> Room::getAllUsers()
{
    vector<string> res;

    for (auto& it : m_users)
        res.push_back(it.getUsername());
    return res;
}