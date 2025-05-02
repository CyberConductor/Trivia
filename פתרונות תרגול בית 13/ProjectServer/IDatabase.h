#pragma once
#include "winsqlite/winsqlite3.h"
#include <string>

using std::string;

class IDatabase
{
public:
    virtual ~IDatabase() = default;

    virtual bool open() = 0;
    virtual bool close() = 0;

    virtual int doesUserExist(string username) = 0;
    virtual int doesPasswordMatch(string username, string password) = 0;
    virtual int addNewUser(string username, string password, string emailAddr) = 0;
};