#include "player_list.h"
#include "pointers.hpp"
#include "gta_util.hpp"
#include "natives.hpp"
#include "features.hpp"
#include "imgui.h"
#include "fiber_pool.hpp"

namespace zyko
{
	void run_playerlist()
	{
		update_playerlist();
		if (gta_util::get_connected_players() != playerlist.size())
		{
			playerlist.clear();
			for (std::uint32_t i = 0; i < 32; i++)
			{	
				if (CNetGamePlayer* net_player = g_pointers->m_get_net_player(i))
				{
					auto cstr_name = net_player->get_name();
					std::string name = cstr_name;
					transform(name.begin(), name.end(), name.begin(), ::tolower);
					playerlist[name] = { cstr_name, i };
				}
			}
		}
	}

	void update_playerlist()
	{
		using namespace features;

		if (CNetGamePlayer* net_player = g_pointers->m_get_net_player(g_selected_player))
		{


			g_player_list.is_modder = is_modder(g_selected_player);
			g_player_list.player_id = net_player->m_player_id;

			g_player_list.coordsx = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true).x;
			g_player_list.coordsy = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true).y;
			g_player_list.coordsz = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true).z;
			g_player_list.ped_id = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_player_list.player_id);
			g_player_list.is_vehicle = PED::IS_PED_IN_ANY_VEHICLE(g_player_list.ped_id, false);
			g_player_list.health = ENTITY::GET_ENTITY_HEALTH(g_player_list.ped_id);
			g_player_list.armour = PED::GET_PED_ARMOUR(g_player_list.ped_id);
			g_player_list.speed = ENTITY::GET_ENTITY_SPEED(g_player_list.ped_id);

			g_player_list.online_ip = net_player->get_net_data()->m_external_ip;
			g_player_list.online_port = net_player->get_net_data()->m_external_port;


		}
	}





}