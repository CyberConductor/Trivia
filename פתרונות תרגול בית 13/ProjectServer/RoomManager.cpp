#include "RoomManager.h"

RoomManager::RoomManager(){}

RoomManager::~RoomManager() { m_rooms.clear(); }

void RoomManager::createRoom(LoggedUser user, RoomData data)
{
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		roomID id = it->first;
		if (id == data.id)//check that there are no room existing with the same id
			return;
	}

	Room room = Room(data);
	room.addUser(user);
	m_rooms.insert({ data.id, room });
}

void RoomManager::deleteRoom(int ID)
{
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
	{
		if (it->first == ID)
		{
			m_rooms.erase(it);
			return;
		}
	}
}

RoomStatus RoomManager::getRoomState(int ID)
{
	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
		if (it->first == ID)
			return it->second.m_metadata.status;

	return false;
}

vector<RoomData> RoomManager::getRooms()
{
	vector<RoomData> res;

	for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it)
		res.push_back( it->second.m_metadata );

	return res;
}

Room* RoomManager::getRoom(int ID)
{
	for (auto& room : m_rooms)
		if (room.first == ID)
			return &room.second;

	return nullptr;
}