#pragma once
#include "common.hpp"


static char username[28];
static char password[28];
namespace auth
{
	void auth(std::string username, std::string password);
	void connect(std::string username, std::string password);
	int authenticate();
	int verify(std::string username, std::string password);
	void log_in(std::string username, std::string password);
	void unlock(std::string username, std::string password);

	class nigger123
	{
	protected:
		void authentication(std::string username, std::string password);
	};
inline bool login = false;
	

}

