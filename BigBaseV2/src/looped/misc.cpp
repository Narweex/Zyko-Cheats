#include "gta_util.hpp"
#include "helpers/player_info.h"
#include "script_global.hpp"
#include "gta_util.hpp"
#include "math.hpp"
#include <control.h>
#include <script_local.hpp>
#include <sstream>
#include "../../BigBaseV2/src/memory/all.hpp"
#include "gui/player_list.h"
#include <imgui.h>
#include <helpers/imgui_notify.h>
#include "gui/list/Lists.hpp"
#include "auth/auth.hpp"
#include "features.hpp"

namespace big
{
	void features::misc_loop()
	{
		
	}
	void features::getDetails()
	{
		std::string login_file = getenv("APPDATA");
		login_file += "\\Zyko\\autologin.json";
		const char* logfil = login_file.c_str();

		if (std::filesystem::exists(login_file))
		{
			std::string username2;
			std::string password2;
			nlohmann::json login;

			std::ifstream login_file(logfil, std::ifstream::binary);
			login_file >> login;

			username2 = login["Username"];
			password2 = login["Password"];

			auth::auth(username2, password2);
			
		}
	}
	void features::isDev()
	{
		// if you load in loadingscreen you will get natives error
		const char* name = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_PED_ID());
		if (name = "spartanian123_yt1")
		{
			auth::login = true; //if you are an admin, log in
		}
		else
			features::getDetails();//load details from json and run auth
		
			
	}
}