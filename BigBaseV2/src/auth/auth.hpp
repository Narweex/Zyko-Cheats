#pragma once


class Auth
{
private:
	 std::string m_username;
	 std::string m_password;
	 
public:
	std::string m_username_check;
	std::string m_password_check;
	Auth(const std::string& username, const std::string& password)
		:m_username(username), m_password(password), m_username_check(username), m_password_check(password)
	{
		
		auth(m_username, m_password);
	};	
	Auth()
	{
		LOG(INFO) << m_username_check << m_password_check;
		auth(m_username_check, m_password_check);
	};
	//void Authenticate(const std::string& username, const std::string& password);
	void auth(const std::string& username, const std::string& password);
public:
	
};



namespace auth
{
	static char username[28];
	static char password[28];
	inline bool login = false;
	static int login1 = 0;
}

