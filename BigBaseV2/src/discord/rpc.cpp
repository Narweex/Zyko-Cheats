#include "features.hpp"
#include "discord_rpc.h"
#include "pointers.hpp"
#include <string>

namespace zyko
{
	void features::DiscordInit()
	{
		if (features::discord_rpc)
		{
			DiscordEventHandlers handlers;
			memset(&handlers, 0, sizeof(handlers));
			Discord_Initialize("968221261559922779", &handlers, TRUE, nullptr);
			UpdatePresence();
		}
		else
		{
			DiscordDestroy();
		}
		
	}

	void features::UpdatePresence()
	{
		if (*g_pointers->m_is_session_started)
		{
			int kek = gta_util::get_connected_players();
			std::ostringstream players;
			players<< "In session with" << kek << "/32 players";
			DiscordRichPresence discordPresence;
			memset(&discordPresence, 0, sizeof(discordPresence));
			discordPresence.details = players.str().c_str();
			discordPresence.startTimestamp = 0;
			discordPresence.largeImageKey = "loginlogo";
			discordPresence.largeImageText = "zykocheats.org";
			discordPresence.smallImageKey = "loginlogo";
			discordPresence.smallImageText = "zykocheats.org";
			discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
			Discord_UpdatePresence(&discordPresence);
		}
		else
		{
			DiscordRichPresence discordPresence;
			memset(&discordPresence, 0, sizeof(discordPresence));
			discordPresence.state = ZykoVersion;
			discordPresence.details = "In Storymode";
			discordPresence.startTimestamp = 0;
			discordPresence.largeImageKey = "loginlogo";
			discordPresence.largeImageText = "Zykocheats.org";
			Discord_UpdatePresence(&discordPresence);
		}
		
	}

	void features::DiscordDestroy()
	{
		Discord_Shutdown();
	}

	

}