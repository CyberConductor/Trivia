#pragma once
#include "Room.h"
#include <map>

using std::map;

typedef int roomID;

class RoomManager
{
public:
	void createRoom(LoggedUser, RoomData);
	void deleteRoom(int ID);
	RoomStatus getRoomState(int ID);
	vector<RoomData> getRooms();
private:
	map<roomID, Room> m_rooms;
};