#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(LoginManager& manager) 
	: m_loginManager(manager)
{
	this->m_database = new SqliteDatabase();
	m_database->open();
}

RequestHandlerFactory::~RequestHandlerFactory()
{
	delete m_database;
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return this->m_loginManager;
}