#pragma once
#include "WinSock2.h"
#include <string>
#include <vector>
#include <iostream>
#include "SqliteDatabase.h"

using std::string;
using std::vector;

class LoggedUser {
public:
	LoggedUser(string, SOCKET);
	string getUsername() const;
	SOCKET getSocket() const;

	bool operator<(const LoggedUser&) const;
private:
	string m_username;
	SOCKET m_socket;
};

class LoginManager
{
public:
	LoginManager();
	~LoginManager();

	bool signup(string username, string password, string email, SOCKET);
	bool login(string username, string password, SOCKET);
	void logout(string);
private:
	IDatabase* m_database;
	vector<LoggedUser> m_loggedUsers;
};