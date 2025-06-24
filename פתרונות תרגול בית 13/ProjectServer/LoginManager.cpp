#include "LoginManager.h"

LoggedUser::LoggedUser(string username) 
	: m_username(username){}

string LoggedUser::getUsername()
{
	return m_username;
}

LoginManager::LoginManager()
{
	m_database = new SqliteDatabase();
	m_database->open();
}

LoginManager::~LoginManager()
{
	delete m_database;
}

bool LoginManager::signup(string username, string password, string email)
{
	int res = m_database->doesUserExist(username);
	if (res == SQLITE_OK)
	{
		res = m_database->addNewUser(username, password, email);
		if (res == SQLITE_OK)
		{
			m_loggedUsers.push_back(LoggedUser(username));
			return true;
		}
	}
	return false;
}

bool LoginManager::login(string username, string password)
{
	// check if user is already logged in
	for (auto& user : m_loggedUsers)
		if (user.getUsername() == username)
			return false; // already logged in

	// check if password matches
	if (m_database->doesPasswordMatch(username, password))
	{
		m_loggedUsers.push_back(LoggedUser(username));
		return true;
	}

	return false;
}

void LoginManager::logout(string username)
{
	for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it)
		if (it->getUsername() == username)
		{
			m_loggedUsers.erase(it);
			break;
		}
}
