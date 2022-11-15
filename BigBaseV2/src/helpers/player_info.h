#pragma once
#include "natives.hpp"
#include "gta\player.hpp"

namespace zyko
{
	static class local_player_info_class
	{
	public:
		static inline Player player_id{};
		static inline Ped player_ped{};
		static bool is_in_vehicle();
		static bool network_is_activity_session();
		static bool is_cutscene_playing();
		static int net_player_id();
		static int net_vehicle_id();
	} g_player_info;
}