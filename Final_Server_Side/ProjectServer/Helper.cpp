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
	int size = bytesToInt(sizeStr);

	if (size < 0)
	{
		info.id = CLIENT_ERROR;
		return info;
	}

	string data = getPartFromSocket(sc, size); // read the buffer
	info.buffer = Buffer(data.begin(), data.end());
	return info;
}

int Helper::bytesToInt(const string& bytes)
{
	unsigned char b0 = bytes[0];
	unsigned char b1 = bytes[1];
	unsigned char b2 = bytes[2];
	unsigned char b3 = bytes[3];
	return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

// send data to socket
// this is private function
void Helper::sendData(SOCKET sc, const Buffer& message)
{
	if (send(sc, reinterpret_cast<const char*>(message.data()), message.size(), 0) == INVALID_SOCKET)
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
	if (bytesNum <= 0)
	{
		return "";
	}

	string result;
	result.resize(bytesNum);
	int received = 0;

	while (received < bytesNum)
	{
		int bytesRead = recv(sc, &result[received], bytesNum - received, flags);
		if (bytesRead <= 0)
		{
			string s = "Error while receiving from socket or connection closed: ";
			s += std::to_string(sc);
			throw std::exception(s.c_str());
		}
		received += bytesRead;
	}

	return result;
}