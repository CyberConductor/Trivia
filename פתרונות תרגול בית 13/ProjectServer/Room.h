#pragma once

#include <string>
#include <vector>
#include <map>
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
	unsigned int timePerQuestion;
	RoomStatus status;
} RoomData;

//forward decleration
class IRequestHandler;

class Room
{
public:
	Room(RoomData);
	~Room();

	void addUser(LoggedUser, IRequestHandler*);
	void removeUser(LoggedUser);
	vector<string> getAllUsers();

	//public field used in RoomManager
	map<LoggedUser, IRequestHandler*> m_users;
	RoomData m_metadata;
private:
	//Helper count
	unsigned int currentUsers;
};