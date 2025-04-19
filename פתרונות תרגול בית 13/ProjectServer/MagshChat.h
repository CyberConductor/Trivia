#pragma once

#include "Helper.h"
#include <string>
#include <mutex>

// Q: why do we need this class ?
// A: a wrap to std::fstream for our need
class MagshChat
{
public:
	MagshChat();
	~MagshChat();
	std::string read(const std::string& firstUsername, const std::string& secondUsername);
	void write(const std::string& firstUsername, const std::string& secondUsername, const std::string& data);

private:
	static std::string get_chat_file_path(const std::string& firstUsername, const std::string& secondUsername);
};

