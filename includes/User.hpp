#ifndef USER_HPP
#define USER_HPP

#include <arpa/inet.h>
#include "Log.hpp"
#include <sstream>
#include <ctime>
#include "Server.hpp"
#include "Channel.hpp"
#include "Command.hpp"

# define BUFFER_SIZE 520
# define MSG_END "\r\n"

enum USER_STATE { CONNECTED, NICK, PASSWORD, REGISTERED, ONLINE, DELETE};
// enum USER_ROLE {CREATOR, OPERATOR, USER}

class Command;

class User
{

private:
	int						_fd;
	int						_port;
	time_t					_last_ping;
	int						_state;

	std::string 			_username;
	std::string 			_nickname;
	std::string				_fullname;
	std::string				_nick_user_host;

	std::vector<std::string> 			_dataToSend;
	std::map<std::string, Channel *> 	channels;
	// Channel* channel;

	std::map<std::string, void(*)(Command *)> command_function;

public:
	std::string							buffer;

	User(int fd, uint16_t port);
	~User();

	std::string getNickname();
	std::string getUsername();
	std::string getFullname() const;
	time_t		getLastPing() const;
	// std::string getPrefix() const;
	// int			getFd() const;

	bool isRegistered() const;

	void setNickname(std::string);
	void setUsername();
	void setFullname();
	void setNickUserHost();
	void setLastPing(time_t last_ping);

	bool getState();
	void receiveData();
	void registerNewUser();
	int getFd();

	void readMessage();
	void write(std::string msg);
	void reply(std::string& reply);

	void welcome() {};
	void join(Channel* channel);
	void leave() {};

	void sendPong();

};

#endif