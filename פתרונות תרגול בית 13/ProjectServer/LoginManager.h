#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "SqliteDatabase.h"

using std::string;
using std::vector;

class LoggedUser {
public:
	LoggedUser(string username);
	string getUsername() const;
private:
	string m_username;
};

class LoginManager
{
public:
	LoginManager();
	~LoginManager();

	bool signup(string username, string password, string email);
	bool login(string username, string password);
	void logout(string);
private:
	IDatabase* m_database;
	vector<LoggedUser> m_loggedUsers;
};