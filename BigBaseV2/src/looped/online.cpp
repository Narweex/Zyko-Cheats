#include "natives.hpp"
#include "features.hpp"

namespace zyko
{
	void features::boostPlayer()
	{
		static int i = 0;
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_selected_player)) && i < 25);
		{
			i++;
		}
		for (int i = 0; i < 20; i++)
		{
		Player playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player);
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 0);
		
			RequestControlOfEnt(veh);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 500);
		}	
	}
	
	bool features::npc_esp = false;
	void features::Npc_esp(bool toggle)
	{
		static int peds;
		PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), peds, 0);
		for (int i = 0; i < peds; i++)
		{
			Vector3 buffer = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PED::GET_PED_NEARBY_PEDS(PLAYER::PLAYER_PED_ID(), peds, 0), 0, 0, 0);
			Vector3 playerCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0, 0);

			GRAPHICS::DRAW_LINE(playerCoords.x, playerCoords.y, playerCoords.z, buffer.x, buffer.y, buffer.z, 255, 0, 0, 255);
		}
	}
	
	bool features::esp_line_all = false;
	bool features::esp_box_all = false;
	bool features::esp_name_all = false;
	bool features::esp_distance_all = false;

	void features::ESP_all()
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return;

		if (features::esp_box_all)
		{
			for (int y = 0; y < gta_util::get_connected_players(); y++)
			{
				Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(y), 0, 0, 0);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255); // top Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 200, 94, 100, 255); // bottom Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 200, 94, 100, 255);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255); // bottom Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);

			}
		}

		if (features::esp_line_all)
		{
			for (int i = 0; i < gta_util::get_connected_players(); i++)
			{
				Vector3 locationOne = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false);
				Vector3 locationTwo = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
				GRAPHICS::DRAW_LINE(locationOne.x, locationOne.y, locationOne.z, locationTwo.x, locationTwo.y, locationTwo.z, 255, 0, 0, 255);

			}
		}

		if (features::esp_name_all)
		{
			for (int i = 0; i < gta_util::get_connected_players(); i++)
			{
				float screenX;
				float screenY;
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 0, 0, 0);
				Player player = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
				if (ENTITY::DOES_ENTITY_EXIST(player))
				{
					BOOL screenCoord = GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(coords.x, coords.y, coords.z, &screenX, &screenY);
					HUD::SET_TEXT_FONT(7);
					HUD::SET_TEXT_SCALE(0.0, 0.30);
					HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
					HUD::SET_TEXT_CENTRE(0);
					HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
					HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
					HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");	
					HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i)));
					HUD::END_TEXT_COMMAND_DISPLAY_TEXT(screenX, screenY, 1);
					HUD::SET_TEXT_OUTLINE();
					HUD::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
				}
			}
		}

		
		
		
	}

	bool features::boxesp = false;
	bool features::line_esp = false;
	void features::Playeresp(bool toggle, uint32_t players)
	{

		if (toggle)
		{
			Player playerHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players);

			if (features::boxesp)
			{

				Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
				const char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(players));

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255); // top Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 200, 94, 100, 255); // bottom Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, 200, 94, 100, 255);

				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255); // bottom Box
				GRAPHICS::DRAW_LINE(handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x + 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y - 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
				GRAPHICS::DRAW_LINE(handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z - 0.75, handleCoords.x - 0.5, handleCoords.y + 0.5, handleCoords.z + 0.75, 200, 94, 100, 255);
			}



			if (features::line_esp)
			{
				Vector3 locationOne = ENTITY::GET_ENTITY_COORDS(playerHandle, false);
				Vector3 locationTwo = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
				GRAPHICS::DRAW_LINE(locationOne.x, locationOne.y, locationOne.z, locationTwo.x, locationTwo.y, locationTwo.z, 255, 0, 0, 255);
			}
				

				
					
					
				
			
		}
	}

	bool features::moneynotify = false;
	void features::Moneynotify(bool toggle)
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return;
		if (toggle)
		{
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Hash drophashes[] = {
				0x1E9A99F8,
				0x113FD533,
			};
			if (OBJECT::_IS_PICKUP_WITHIN_RADIUS(*drophashes, coords.x, coords.y, coords.z, 9999.0f))
			{
				Notify("Detected Money Drop !", "Someone Is Dropping Money In Session", 5000, None);

			}
		}
	}

	bool features::spectateplayer = false;
	void features::Spectateplayer(bool toggle)
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return;
		if (toggle) 
		{
			GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.25, 0.25, 0.25, 200, 94, 100, 255, 1, 1, 1, 0, 0, 0, 0);
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		}
		else {
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, PLAYER::PLAYER_PED_ID());
		}
	}

	bool features::fucktheircam = false;
	void features::Fucktheircam(bool toggle)
	{
		if (toggle)
		{
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
			FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 0.f, false, true, 10000.f, true);
		}
	}

	bool features::freeze_player = false;
	void features::Freeze_player(bool toggle)
	{
		if (toggle)
		{
			features::RequestControlOfEnt(features::g_selected_player);
			ENTITY::FREEZE_ENTITY_POSITION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		}
	}

	bool features::nophone = false;
	void features::Nophone(bool toggle)
	{
		if (toggle)
		{
			*script_global(21285).as<int*>() = 6;
			*script_global(19954 + 1).as<int*>() = 3;
		}
	}
		

	
	bool features::rockstaradmin = false;
	void features::admindetection(bool toggle)
	{
		if (!NETWORK::NETWORK_IS_SESSION_ACTIVE())
			return;
		if (toggle)
		{

		
		const char* handle{};
		for (int i = 0; i < 32; i++)
		{
			handle = PLAYER::GET_PLAYER_NAME(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i));

		}
			if (handle = *Lists::R1Admins)
			{
				Notify("Admin Detected", "You should leave the session immediatelly", 7000, Protections);

				if (features::leaveondetect)
				{
					Notify("Changing Session", "Changed because of admin detection", 7000, Protections);
					features::change_session(FindPublicSession);

				}
				else if (features::crashgame)
				{
					exit(0);
				}
			}
		}
	}

	bool features::notify_on_join = false;
	void features::Notify_on_join(bool toggle)
	{
		/*for (int i; i < 32; i++)
		{
			if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) && ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))) {
				const char* name = PLAYER::GET_PLAYER_NAME(i);

				features::notify(name, "Player Joining", 7000);
			
			}
		}*/
	}

	bool features::ragdoll_player = false;
	void features::Ragdoll_player()
	{
		PED::SET_PED_CAN_RAGDOLL(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		PED::SET_PED_TO_RAGDOLL(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1, 1, 0, 1, 1, 0);
	}

	bool features::kick_from_mk2 = false;
	void features::Kick_from_mk2(bool toggle)
	{
		if (toggle)
		{
			for (int i = 0; i < 32; i++)
			{
				if (ENTITY::DOES_ENTITY_EXIST(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) && PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) != PLAYER::PLAYER_PED_ID()) //if player exists and is not user
				{
					Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)); //get their vehicle
					if (VEHICLE::IS_VEHICLE_MODEL(veh, MISC::GET_HASH_KEY("oppressor2"))) // check for oppressors
					{
						const char* name = PLAYER::GET_PLAYER_NAME(i);
						Notify(name, "Exploding his oppressor", 7000, Protections);
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false);
						FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 50.f, false, true, 1000.f, false);
						
					}
				}
			}
		}
	}
}
