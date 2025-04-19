#include "MagshChat.h"
#include <fstream>

MagshChat::MagshChat()
{

}

MagshChat::~MagshChat()
{
}

// the funcion reads content of shared file
// if the file does not exist return empty string
// the function is thread-safe: use mutex to lock the file
std::string MagshChat::read(const std::string& firstUsername, const std::string& secondUsername)
{
	const int SIZE = 100;

	std::ifstream doc;
	std::string res;
	const std::string file_path = get_chat_file_path(firstUsername, secondUsername);

	doc.open(file_path.c_str());
		

	if (!doc.is_open())
	{
		return "";
	}

	std::string line;
	while (!doc.eof())
	{
		std::getline(doc, line);
		//doc.getline(output, SIZE);
		res += line;
		if (!doc.eof())
		{
			res.append("\n\r");
		}
	}

	doc.close();

	return res;
}

std::string MagshChat::get_chat_file_path(const std::string& firstUsername, const std::string& secondUsername)
{
	if (firstUsername.compare(secondUsername) > 0)
	{
		return firstUsername + "&" + secondUsername + ".txt";
	}
	return secondUsername + "&" + firstUsername + ".txt";
}


void MagshChat::write(const std::string& firstUsername, const std::string& secondUsername, const std::string& data)
{
	std::ofstream doc;
	const std::string messageMagic = "&MAGSH_MESSAGE&";
	const std::string file_path = get_chat_file_path(firstUsername, secondUsername);
	// Append data to file
	doc.open(file_path, std::ios::out | std::ios::app);
	doc.write(messageMagic.c_str(), messageMagic.length());
	doc.write("&Author&", strlen("&Author&"));
	doc.write(firstUsername.c_str(), firstUsername.length());
	doc.write("&DATA&", strlen("&DATA&"));
	doc.write(data.c_str(), data.length());
	doc.flush();
	doc.close();
}
