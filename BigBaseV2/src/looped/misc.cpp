#include "../common.hpp"
#include "features.hpp"

namespace big
{
	void features::misc_loop()
	{
		
	}
	
		void features::isDev()
		{
			// if you load in loadingscreen you will get natives error
			const char* name = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_PED_ID());
			if (name = "spartanian123_yt")
			{
				features::notify("Access granted", "You are Developer", 7000);
				auth::login = true; //if you are an admin, log in
			}
		}
	
}