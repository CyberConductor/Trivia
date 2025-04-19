#include "RecvMessage.h"

RecvMessage::RecvMessage(const SOCKET sock, const int messageCode)
{
	_sock = sock;
	_messageCode = messageCode;
}

RecvMessage::RecvMessage(const SOCKET sock, const int messageCode, std::vector<std::string> values) : RecvMessage(sock, messageCode)
{
	_values = values;
}

SOCKET RecvMessage::getSock()
{
	return _sock;
}


int RecvMessage::getMessageCode()
{
	return _messageCode;
}


std::vector<std::string>& RecvMessage::getValues()
{
	return _values;
}