#include "../includes/User.hpp"

/* ======================================================================================== */
/* ----------------------------- CONSTRUCTOR / DESTRUCTOR --------------------------------  */
User::~User() {}
User::User(int fd, sockaddr_in u_address, Server* server)
	:	_server(server), _hostname(HOSTNAME),
		_port(ntohs(server->getAddr().sin_port)),
		_user_address(u_address),
		_fd(fd), _state(CONNECTED),
		_username(), _nickname(), _nick_user_host(), _nickOP(""),
		_password(),
		_buffer(), _dataToSend(),
		command_function(), _first_nick(false) {};

/* ======================================================================================== */
/* --------------------------------------- GETTER ----------------------------------------  */
int User::getFd() const{ return this->_fd; }
int User::getState() const { return this->_state; }
std::string	User::getNickUserHost() const{ return this->_nick_user_host; }
std::string	User::getUsername() const { return this->_username; }
std::string User::getNickname() const { return this->_nickname; }
std::string User::getPassword() const { return this->_password; }
std::string User::getUserChannelStatus() const { return this->_userStatusInChannel; }
std::string User::getNicknameOP() const { return this->_nickOP; }

/* ======================================================================================== */
/* --------------------------------------- SETTER ----------------------------------------  */
void User::setPassword(std::string pw) { this->_password = pw; }
void User::setState(int new_state) { this->_state = new_state; }
void User::setNicknameOP(std::string nickOP){this->_nickOP = nickOP;}
void User::setUsername(const std::string& username) { this->_username = username; }
void User::setUserChannelStatus(const std::string &status) {this->_userStatusInChannel = status;}

void User::setNickname(const std::string& nick)
{
	this->_nickname = nick;
	this->setNickUserHost();
}

void	User::setNickUserHost() {
	this->_nick_user_host.clear();
	this->_nick_user_host.append(":");
	this->_nick_user_host.append(this->getNickname());
	this->_nick_user_host.append("!");
	this->_nick_user_host.append(this->getNickname());
	this->_nick_user_host.append("@");
	this->_nick_user_host.append(HOSTNAME);
}

/* ======================================================================================== */
/* -------------------------------------- MAIN LOOP --------------------------------------  */
/* Function gets called when there is data to receive for the user. */
void User::onUser(void)
{
	if (receive() == false)
		return ;

	split();
	invoke();
	write();
	clearCommandFunction();
}

/* ======================================================================================== */
/* ---------------------- RECEIVE / PARSE / INVOKE / SEND MESSAGES -----------------------  */
/* User receives data with recv() and saves the read bytes within buffer string. */
bool User::receive(void)
{
	char recv_buffer[BUFFER_SIZE + 1];
	memset(&recv_buffer, 0, sizeof(BUFFER_SIZE + 1));

	size_t size = recv(this->_fd, &recv_buffer, BUFFER_SIZE, 0);
	if (size < 0)
		return false;
	else if (size == 0)
	{
		setState(DELETE);
		return false;
	}

	recv_buffer[size] = 0;
	this->_buffer.append(recv_buffer);

	return true;
}

/* Splits messages when separated by END_MSG ("\n"). */
void User::split(void)
{
	size_t pos;
	std::string delimiter("\n");

	while ((pos = this->_buffer.find(delimiter)) != std::string::npos)
	{
		std::string tmp = this->_buffer.substr(0, pos);
		this->_buffer.erase(0, pos + delimiter.length());
		this->_dataToSend.push_back(tmp);
	}
}

/* Create a Command object for every valid input string and push it into the vector. */
void User::invoke(void)
{
	for (std::vector<std::string>::iterator iter = _dataToSend.begin(); iter != _dataToSend.end(); iter++)
		this->command_function.push_back(new Command(this, this->_server, *iter));

	this->_dataToSend.clear();
}

/* Loop over vector and execute commands that are ready to send and send replies when appropriate. */
void User::write(void)
{
	for (std::vector<Command*>::iterator iter = command_function.begin(); iter != command_function.end(); iter++)
	{
		if ((*iter)->getCommandState() == true)
		{
			if (send((*iter)->getReceiverFd(), (*iter)->getCommandMessage().c_str(), (*iter)->getCommandMessage().length(), 0) < 0)
				Log::printStringCol(CRITICAL, "ERROR: SENDING MESSAGE FROM USER FAILED.");
			else
				Log::printStringCol(LOG, "LOG: SENDING COMMAND SUCCESSFULLY");
		}

		if ((*iter)->getReplyState() == true)
		{
			if (send(this->_fd, (*iter)->getReply().c_str(), (*iter)->getReply().length(), 0) < 0)
				Log::printStringCol(CRITICAL, "ERROR: SENDING REPLY TO USER FAILED.");
			else
				Log::printStringCol(LOG, "LOG: SENDING REPLY SUCCESSFULLY");
		}
	}
}

/* ======================================================================================== */
/* --------------------------------------- CLEAR -----------------------------------------  */
void User::clearCommandFunction(void)
{
	for (std::vector<Command *>::iterator iter = command_function.begin(); iter != command_function.end(); iter++)
		delete *iter;

	command_function.clear();
}

/* ======================================================================================== */
/* ----------------------------------- OTHER CHECKS --------------------------------------  */
bool User::isRegistered() const
{
	if (this->_nickname.length() != 0 && this->_username.length() != 0 && this->_password == this->_server->getPassword())
		return true;

	return false;
}
