#pragma once
#include "CNetGamePlayer.hpp"

namespace big
{
	struct player_info
	{
		const char* name{};
		std::uint32_t id{};
		bool is_modder{};
		std::uint8_t player_id{};
		Ped ped_id{};
		BOOL is_vehicle{};
		float health{};
	};
    void run_playerlist();
	void update_playerlist();
	inline std::map<std::string, player_info> playerlist;
	inline struct player_info g_player_list;
}