#include "LoginManager.h"

LoggedUser::LoggedUser(string username, SOCKET sock) 
	: m_username(username),
	m_socket(sock)
{}

string LoggedUser::getUsername() const { return m_username; }
SOCKET LoggedUser::getSocket() const { return m_socket; }

bool LoggedUser::operator<(const LoggedUser& other) const
{
	return m_username < other.getUsername();
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

bool LoginManager::signup(string username, string password, string email, SOCKET sock)
{
	if (!m_database->doesUserExist(username))
	{
		if (m_database->addNewUser(username, password, email) == SQLITE_OK)
		{
			std::cout << "new client added\n";
			m_loggedUsers.push_back(LoggedUser(username, sock));
			return true;
		}
	}
	std::cout << "existing user already existing\n";
	return false;
}

bool LoginManager::login(string username, string password, SOCKET sock)
{
	// check if user is already logged in
	for (auto& user : m_loggedUsers)
		if (user.getUsername() == username)
			return false; // already logged in

	// check if password matches
	if (m_database->doesPasswordMatch(username, password))
	{
		m_loggedUsers.push_back(LoggedUser(username, sock));
		return true;
	}

	return false;
}

void LoginManager::logout(string username)
{
	for (auto it = m_loggedUsers.begin(); it != m_loggedUsers.end(); ++it)
		if (it->getUsername() == username)
		{
			closesocket(it->getSocket());
			m_loggedUsers.erase(it);
			break;
		}
}
