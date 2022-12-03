#include "Server.hpp"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Error: Invalid paramters. Usage: ./ircserv <port> <password>" << std::endl;
		return -1;
	}

	Server server = Server();
	server.init(argv);

	// while (true)
	server.execute(); //here poll loop
	sleep(1);

	return 0;
}