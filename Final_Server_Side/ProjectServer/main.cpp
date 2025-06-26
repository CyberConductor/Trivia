#include "Server.h"
#include <iostream>
#include <string>
#include "WSAInitializer.h"
#include <fstream>
#include <thread>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::thread;

int main()
{
	try
	{
		TRACE("Starting...");
		WSAInitializer wsa_init;

		Server md_server;

		// launch the server in a separate thread
		thread serverThread(&Server::run, &md_server);
		serverThread.detach(); // allows main thread to stay responsive

		// input loop for admin
		string userInput;
		while (true)
		{
			std::getline(cin, userInput);

			if (userInput == "EXIT" || userInput == "exit")
			{
				TRACE("Exiting server...");
				break;
			}
		}
	}
	catch (const std::exception& e)
	{
		cout << "Exception was thrown in function: " << e.what() << endl;
	}
	catch (...)
	{
		cout << "Unknown exception in main!" << endl;
	}
}