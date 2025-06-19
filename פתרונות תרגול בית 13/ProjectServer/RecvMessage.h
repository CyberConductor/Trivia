#pragma once

#include <string>
#include <vector>
#include <Windows.h>

using std::string;
using std::vector;

class RecvMessage
{
public:
	RecvMessage(const SOCKET, const int messageCode);
	RecvMessage(const SOCKET, const int messageCode, const vector<string> values);

	SOCKET getSock();
	int getMessageCode();
	vector<string>& getValues();
private:
	SOCKET _sock;
	int _messageCode;
	vector<string> _values;
};

