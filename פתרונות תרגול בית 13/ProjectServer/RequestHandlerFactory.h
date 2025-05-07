#pragma once
#include "LoginRequestHandler.h"
#include "LoginManager.h"
#include "SqliteDatabase.h"

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(const LoginManager& manager);
	~RequestHandlerFactory();

	LoginRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();
private:
	LoginManager m_loginManager;
	IDatabase* m_database;
};