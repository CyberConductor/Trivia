#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

//a request info method
RequestInfo Helper::getRequestInfo(const SOCKET sc)
{
	RequestInfo info;
	info.receivalTime = time(nullptr);

	string idStr = getPartFromSocket(sc, 1); // read 1 byte for ID
	info.id = static_cast<unsigned char>(idStr[0]);

	string sizeStr = getPartFromSocket(sc, 4); // read 4 bytes for message size

	int size = std::stoi(sizeStr);
	if (size < 0)
	{
		info.id = CLIENT_ERROR;
		return info;
	}

	string data = getPartFromSocket(sc, size); // read the buffer
	info.buffer = Buffer(data.begin(), data.end());
	return info;
}

// send data to socket
// this is private function
void Helper::sendData(const SOCKET sc, const string message)
{
	const char* data = message.c_str();

	if (send(sc, data, message.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
}

// recieve data from socket according byteSize
// this is private function
string Helper::getPartFromSocket(const SOCKET sc, const int bytesNum)
{
	return getPartFromSocket(sc, bytesNum, 0);
}

string Helper::getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags)
{
	if (bytesNum == 0)
	{
		return "";
	}

	char* data = new char[bytesNum + 1];
	int res = recv(sc, data, bytesNum, flags);
	if (res == INVALID_SOCKET)
	{
		string s = "Error while recieving from socket: ";
		s += std::to_string(sc);
		throw std::exception(s.c_str());
	}
	data[bytesNum] = 0;
	string received(data);
	delete[] data;
	return received;
}
