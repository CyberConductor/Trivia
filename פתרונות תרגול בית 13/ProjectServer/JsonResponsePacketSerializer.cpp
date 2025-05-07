#include "JsonResponsePacketSerializer.h"
#include <string>
#include <vector>
#include <cstring>

using json = nlohmann::json;

Buffer JsonResponsePacketSerializer::serializeResponse(ErrorResponse response)
{
	json j;
	j["message"] = response.message;
	string jsonStr = j.dump();
	unsigned int size = jsonStr.size();

	Buffer buffer;
	buffer.push_back(0); // message code for ErrorResponse
	buffer.push_back(static_cast<unsigned char>((size >> 24) & 0xFF));
	buffer.push_back(static_cast<unsigned char>((size >> 16) & 0xFF));
	buffer.push_back(static_cast<unsigned char>((size >> 8) & 0xFF));
	buffer.push_back(static_cast<unsigned char>(size & 0xFF));
	buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(LoginResponse response)
{
	json j;
	j["status"] = response.status;
	string jsonStr = j.dump();
	unsigned int size = jsonStr.size();

	Buffer buffer;
	buffer.push_back(1); // message code for LoginResponse
	buffer.push_back(static_cast<unsigned char>((size >> 24) & 0xFF));
	buffer.push_back(static_cast<unsigned char>((size >> 16) & 0xFF));
	buffer.push_back(static_cast<unsigned char>((size >> 8) & 0xFF));
	buffer.push_back(static_cast<unsigned char>(size & 0xFF));
	buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
	return buffer;
}

Buffer JsonResponsePacketSerializer::serializeResponse(SignupResponse response)
{
	json j;
	j["status"] = response.status;
	string jsonStr = j.dump();
	unsigned int size = jsonStr.size();

	Buffer buffer;
	buffer.push_back(2); // message code for SignupResponse
	buffer.push_back(static_cast<unsigned char>((size >> 24) & 0xFF));
	buffer.push_back(static_cast<unsigned char>((size >> 16) & 0xFF));
	buffer.push_back(static_cast<unsigned char>((size >> 8) & 0xFF));
	buffer.push_back(static_cast<unsigned char>(size & 0xFF));
	buffer.insert(buffer.end(), jsonStr.begin(), jsonStr.end());
	return buffer;
}
