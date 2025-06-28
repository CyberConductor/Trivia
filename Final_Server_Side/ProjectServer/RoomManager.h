#pragma once
#include "Room.h"
#include "IRequestHandler.h"
#include <map>

using std::map;

typedef int roomID;

//forward decleration
class RequestHandlerFactory;

class RoomManager
{
public:
	RoomManager(RequestHandlerFactory*);
	~RoomManager();

	Room& createRoom(LoggedUser, RoomData);
	void deleteRoom(int);
	RoomStatus getRoomState(int);
	vector<RoomData> getRooms();
	Room& getRoom(int);

	//Helpers
	int getFreeId();
<<<<<<< HEAD:פתרונות תרגול בית 13/ProjectServer/RoomManager.h
	//Room& getUserRoom(string);
=======
	Room& getUserRoom(string);
>>>>>>> origin/develop:Final_Server_Side/ProjectServer/RoomManager.h
private:
	map<roomID, Room> m_rooms;
	RequestHandlerFactory* m_handlerFactory;
};