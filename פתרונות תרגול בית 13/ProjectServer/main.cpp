#include "Server.h"
#include <iostream>
#include <string>
#include "WSAInitializer.h"
#include <fstream>

using std::string;
using std::cin;
using std::cout;
using std::endl;

// In a lot of places in the code we pass to function constant reference (const Bla&)
// to an object and not the object itself, 

int main()
{
	// Q: why is this try necessarily ? 
	// A: we want to know what happened that made the server crash
	try
	{
		TRACE("Starting...");
		// NOTICE at the end of this block the WSA will be closed 
		WSAInitializer wsa_init;
		Server md_server;

		// Infinite loop to listen for user input
		string userInput;
		while (true)
		{
			std::getline(cin, userInput);  // Read user input

			if (userInput == "EXIT" || userInput == "exit")
			{
				TRACE("Exiting server...");
				break;  // Exit the loop and end the program
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

