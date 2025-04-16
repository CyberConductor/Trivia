#pragma once

#include <string>
#include <vector>
#include <Windows.h>

class RecvMessage
{
public:
	RecvMessage(const SOCKET sock, const int messageCode);

	RecvMessage(const SOCKET sock, const int messageCode, const std::vector<std::string> values);

	SOCKET getSock();
	int getMessageCode();

	std::vector<std::string>& getValues();

private:
	SOCKET _sock;
	int _messageCode;
	std::vector<std::string> _values;
};

