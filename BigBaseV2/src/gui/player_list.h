#pragma once
#include "CNetGamePlayer.hpp"

namespace big
{
	struct player_info
	{
		const char* name{};
		std::uint8_t id{};
		Ped ped_id{};
		float health{};
		bool modder_notified{ false };
		std::string modder_level{};
	};
    void run_playerlist();
	inline std::map<std::string, player_info> playerlist;
}