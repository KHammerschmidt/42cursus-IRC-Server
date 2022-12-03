#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include <netinet/in.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include <arpa/inet.h>

#define _XOPEN_SOURCE_EXTENDED 1
// special behaviour for C++ (use feature to test macro)
// to use ip_mreq_structure use: #define _XOPEN_SOURCE 500 or #define _OPEN_SYS_SOCK_EXT3

// class User;
// class Channel;

// namespace irc
// {

class Server
{

private:
	int						_port;
	std::string 			_password;
	int						_socket;

	std::vector<pollfd> 	pollfds;			//pollfds iterator?

	// struct sockaddr_in serv_address;
	// std::string 	name;
	// std::string 	admin_name;
	// std::string 	admin_pw;
	// bool			op_rights;

public:
	typedef std::vector<pollfd>::iterator pollfds_iterator;
	// std::map<int, User*> user_db;				//	(name, pw, nickname)
	// std::map<std::string, Channel*> channel_db;	// members, ops, name,

public:
	Server(char** argv);
	~Server();

	int					set_port(const std::string& port_str);
	const std::string&	set_password(const std::string& pw);
	int 				new_socket(void);

	void	execute();

};

// } // end namespace ft
#endif