#pragma once
#include "common.hpp"
#include "../../../vendor/GTAV-Classes/netPlayerData.hpp"
#include "../../../vendor/GTAV-Classes/CNetGamePlayer.hpp"


namespace zyko
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
		float coordsx{};
		float coordsy{};
		float coordsz{};
		float armour{};
		int wanted{};
		float is_dead{};
		int speed{};
		int alpha{};
		Hash zone{};
		rage::netAddress online_ip{};
		uint16_t online_port{};
	};
	void run_playerlist();
	void update_playerlist();
	inline std::map<std::string, player_info> playerlist;
	inline struct player_info g_player_list;
}