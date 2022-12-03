#include "Server.hpp"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Error: Invalid paramters. Usage: ./ircserv <port> <password>" << std::endl;
		return -1;
	}

	Server server(argv);


	while (server.getStatus())
	{
		server.execute(); //here poll loop
	}


	return 0;
}