#include "../includes/Log.hpp"

void Log::printStringCol(int state, std::string msg)
{
	switch(state)
	{
		case REGULAR:
			std::cout << BLUE << msg;
			break;
		case LOG:
				std::cout << GREEN << msg;
				break;
		case WARNING:
				std::cout << YELLOW << BOLD << msg;
				break ;
		case CRITICAL:
				std::cout << RED << BOLD << msg;
				break;
		default:
			std::cout << msg;
			break;
	}
	std::cout << std::endl;
}

void Log::printStringNoNewline(std::string msg)
{
	std::cout << msg;
}

void Log::printTrace(std::string msg)
{
	std::cout << YELLOW << BOLD << "TRACE: " << msg.substr(0, msg.length()) << std::endl;

}

template <typename T>
void Log::print_vector(std::vector<T> vctr)
{
	typename std::vector<T>::iterator iter;
	for (iter = vctr.begin(); iter != vctr.end(); iter++)
		std::cout << *iter << std::endl;
}
