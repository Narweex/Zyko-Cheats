#pragma once


class Auth
{
private:
	 std::string m_username;
	 std::string m_password;

public:
	Auth(const std::string& username, const std::string& password)
	{
		m_username = username;
		m_password = password;
		auth(m_username, m_password);
	};	
	//void Authenticate(const std::string& username, const std::string& password);
	void auth(const std::string& username, const std::string& password);
};



namespace auth
{
	static char username[28];
	static char password[28];
	inline bool login = true;
}

