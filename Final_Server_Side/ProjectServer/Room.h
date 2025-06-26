#pragma once

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "LoginManager.h"

using std::string;
using std::vector;
using std::map;

typedef bool RoomStatus;

typedef struct RoomData
{
	unsigned int id;
	string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	time_t timePerQuestion;
	RoomStatus status;
} RoomData;

//forward decleration
class IRequestHandler;
class RoomAdminRequestHandler;
class Game;
class Communicator;

class Room
{
public:
	Room(RoomData);
	~Room();

	void addUser(LoggedUser, IRequestHandler*);
	void removeUser(LoggedUser);
	vector<string> getAllUsers();

	//public field used in RoomManager
	RoomData m_metadata;
private:
	map<LoggedUser, IRequestHandler*> m_users;
	//Helper count
	unsigned int currentUsers;

	//give access so it will be able to contact all the users in the room
	friend RoomAdminRequestHandler;
	friend Game;
	friend Communicator;
};