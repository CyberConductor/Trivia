#pragma once

#include "LoginManager.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

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

class Room
{
public:
	Room(RoomData);

	void addUser(LoggedUser);
	void removeUser(LoggedUser);
	vector<string> getAllUsers();

private:
	RoomData m_metadata;
	vector<LoggedUser> m_users;
	unsigned int currentUsers;
};