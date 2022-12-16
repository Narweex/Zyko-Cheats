#pragma once


class Auth
{
private:
	 std::string m_username;
	 std::string m_password;
	 
public:
	std::string m_username_check;
	std::string m_password_check;
	Auth()
	{
		static int nigger;
		
		
			std::string login_file = getenv("APPDATA");
			login_file += "\\Zyko\\autologin.json";
			const char* logfil = login_file.c_str();

			std::string username;
			std::string password;
			nlohmann::json login;

			std::ifstream login_file1(login_file);
			login_file1 >> login;

			username = login["Username"];
			password = login["Password"];
			
		
		nigger = auth(username, password);
	
		if (nigger != 1)
		{
			exit(420);
		}
	};	
	
	
	int auth(const std::string& username, const std::string& password);
	
public:
	
};



namespace auth
{
	static char username[28];
	static char password[28];
	inline bool login = true;
	inline bool login1 = true;

}

