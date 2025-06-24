#include "RoomManager.h"
#include <cstdlib>
#include <ctime>

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

int RoomManager::getFreeId()
{
	if (m_rooms.size() >= 1000)
		return -1;
	while (true)
	{
		srand(time(0)); // Seed the random number generator
		int randId = (rand() % 1000) + 1; // Generate a random number in range 0-1000

		//if the id doesn't exist, return it.
		if (m_rooms.find(randId) == m_rooms.end())
			return randId;
	}
}