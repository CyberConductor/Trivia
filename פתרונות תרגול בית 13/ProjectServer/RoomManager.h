#pragma once
#include "Room.h"
#include <map>

using std::map;

typedef int roomID;

class RoomManager
{
public:
	RoomManager();
	~RoomManager();
	void createRoom(LoggedUser, RoomData);
	void deleteRoom(int);
	RoomStatus getRoomState(int);
	vector<RoomData> getRooms();
	Room* getRoom(int);
private:
	map<roomID, Room> m_rooms;
};