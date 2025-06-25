#include "RoomManager.h"
#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <cstdlib>
#include <ctime>

RoomManager::RoomManager(RequestHandlerFactory* factory) : m_handlerFactory(factory){}

RoomManager::~RoomManager() 
{ 
	m_rooms.clear(); 
	delete(m_handlerFactory);
}

Room& RoomManager::createRoom(LoggedUser user, RoomData data)
{
	// check if room with the same ID already exists
	if (m_rooms.find(data.id) != m_rooms.end())
		throw std::runtime_error("Room with this ID already exists");

	data.status = false;

	// insert a room directly into the map to avoid local object lifetime issue
	auto inserted = m_rooms.emplace(data.id, Room(data));
	Room& room = inserted.first->second;

	// set the admin handler
	room.addUser(user, m_handlerFactory->createRoomAdminRequestHandler(user, room));

	return room;
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
		if(it->second.getAllUsers().size() < it->second.m_metadata.maxPlayers)//check if there a place avilable for more players
			res.push_back( it->second.m_metadata );

	return res;
}

Room& RoomManager::getRoom(int ID)
{
	for (auto& room : m_rooms)
		if (room.first == ID)
			return room.second;

	throw std::runtime_error("Room with given ID not found");
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

//Room& RoomManager::getUserRoom(string username)
//{
//	for (auto& room : m_rooms)
//	{
//		map<LoggedUser, IRequestHandler*> users = room.second.m_users;
//		for (auto& user : users)
//			if(user.first.getUsername() == username)
//				return room.second;
//	}
//}