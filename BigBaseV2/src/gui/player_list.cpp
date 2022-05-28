#include "common.hpp"
#include "player_list.h"
#include "pointers.hpp"
#include "gta_util.hpp"
#include "natives.hpp"

namespace big
{
	void run_playerlist()
	{
		if (gta_util::get_connected_players() != playerlist.size())
		{
			playerlist.clear();
			for (std::uint32_t i = 0; i < gta::num_players; i++)
			{
				if (CNetGamePlayer* net_player = g_pointers->m_get_net_player(i))
				{
					auto index = net_player->m_player_id;
					auto cstr_name = net_player->get_name();
					std::string name = cstr_name;
					transform(name.begin(), name.end(), name.begin(), ::tolower);
					auto ped_index = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(index);
					auto health = net_player->m_player_info->m_ped->m_health;

					playerlist[name] = { cstr_name, index, ped_index, health };
				}
			}
		}
	}
}