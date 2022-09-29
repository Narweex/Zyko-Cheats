#include "../common.hpp"
#include "features.hpp"

namespace big
{
	void features::online_loop()
	{
		if (moneynotify)
		{
			if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
				return;
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

			*script_global(21285).as<int*>() = 6;
			*script_global(19954 + 1).as<int*>() = 3;
		}

		if (spectateplayer) {
			if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
				return;
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


		if (features::freeze_player)
		{
			features::RequestControlOfEnt(features::g_selected_player);
			ENTITY::FREEZE_ENTITY_POSITION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		}

	}

	void features::admindetection()
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return;
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
	void features::joinNotification()
	{
		for (int i; i < 32; i++)
		{
			if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) && ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))) {
				const char* name = PLAYER::GET_PLAYER_NAME(i);

				features::notify(name, "Player Joining", 7000);
			
			}
		}
	}

	void features::ragdoll_player()
	{
		PED::SET_PED_CAN_RAGDOLL(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		PED::SET_PED_TO_RAGDOLL(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1, 1, 0, 1, 1, 0);
	}
	void features::kick_from_mk2()
	{
		if (features::kick_from_oppressor)
		{
			for (int i = 0; i < 32; i++)
			{
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) && PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) != PLAYER::PLAYER_PED_ID()) //if player exists and is not user
				{
					Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)); //get their vehicle
					if (VEHICLE::IS_VEHICLE_MODEL(veh, MISC::GET_HASH_KEY("oppressor2"))) // check for oppressors
					{
						const char* name = PLAYER::GET_PLAYER_NAME(i);
						features::notify(name, "Oppressor mk II user", 7000);
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false);
						FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 50.f, false, true, 1000.f, false);
					}
				}


			}
		}
	}
}
