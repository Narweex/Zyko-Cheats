#include "../common.hpp"
#include "features.hpp"
#include "logger.hpp"
#include "natives.hpp"
#include "script.hpp"
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
namespace big
{
	void features::online_loop()
	{
		if (moneynotify)
		{
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Hash drophashes[] = {
				0x1E9A99F8,
				0x113FD533,
			};
			if (OBJECT::_IS_PICKUP_WITHIN_RADIUS(*drophashes, coords.x, coords.y, coords.z, 9999.0f))
			{
				notify_protections("Detected Money Drop !", "Someone Is Dropping Money In Session", 4000);

			}
		}
		if (nophone)
		{
			//PAD::DISABLE_CONTROL_ACTION(0, 27, true);

			*script_global(21285).as<int*>() = 6;
			*script_global(19954 + 1).as<int*>() = 3;
		}

		if (spectateplayer) {
			GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.25, 0.25, 0.25, 200, 94, 100, 255, 1, 1, 1, 0, 0, 0, 0);
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		}
		else {
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, PLAYER::PLAYER_PED_ID());
		}

		if (fucktheircam)
		{
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
			FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 0.f, false, true, 10000.f, true);
		}
		if (features::notifyadmin)
		{
		}
	}

	void features::admindetection()
	{
		const char* handle{};
		for (int i = 0; i < 32; i++)
		{
			handle = PLAYER::GET_PLAYER_NAME(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i));

		}
		if (handle = *Lists::R1Admins)
		{

			features::notify_protections("!!! Rockstar Admin Detected !!!", "You Should Leave Session Immediatelly", 7000);
			if (features::leaveondetect)
			{
				features::notify("Changing Session", "Changed because of admin detection", 7000);
				features::change_session(FindPublicSession);

			}
			else if (features::crashgame)
			{
				exit(0);
			}
		}


	}
}
