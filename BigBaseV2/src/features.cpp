#include "common.hpp"
#include "features.hpp"
#include "logger.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "gta_util.hpp"
#include "helpers/player_info.h"
#include "script_global.hpp"
#include "gta_util.hpp"

namespace big
{
	void features::on_present()
	{
		TRY_CLAUSE
		{
		}
		EXCEPT_CLAUSE
	}

	void drawstring(char* text, float X, float Y)
	{
		//HUD::_SET_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::SET_TEXT_FONT(0);
		HUD::SET_TEXT_SCALE(0.3, 0.3f);
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
		HUD::SET_TEXT_WRAP(0.0f, 1.0f);
		HUD::SET_TEXT_CENTRE(0);
		HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		HUD::SET_TEXT_OUTLINE();
		//HUD::_DRAW_TEXT(X, Y);
	}
	void setupdraw()
	{
		HUD::SET_TEXT_FONT(0);
		HUD::SET_TEXT_SCALE(0.4f, 0.4f);
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
		HUD::SET_TEXT_WRAP(0.0f, 1.0f);
		HUD::SET_TEXT_CENTRE(0);
		HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		HUD::SET_TEXT_OUTLINE();
	}
	void notifyMap(char* fmt, ...)
	{
		char buf[2048] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buf, fmt, va_alist);
		va_end(va_alist);

		char buff2[2048] = { 0 };
		sprintf_s(buff2, "%s", buf);

		HUD::SET_TEXT_OUTLINE();
		//HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buff2);
		//HUD::_DRAW_NOTIFICATION(FALSE, FALSE);
	}	void notifyMap(std::string str) { notifyMap(&str[0]); }

	void features::basket_transaction(int cat, int action, int flag, std::vector<std::array<int, 5>> items)
	{
		if (NETSHOPPING::NET_GAMESERVER_BASKET_END())
			NETSHOPPING::_NET_GAMESERVER_BASKET_DELETE();

		int transaction = -1;

		if (g_pointers->m_construct_basket(*g_pointers->m_transact_queue, &transaction, cat, action, flag))
		{
			for (auto& item : items)
			{
				g_pointers->m_add_item_to_basket(*g_pointers->m_transact_queue, item.data());
			}
			g_pointers->m_process_transaction(*g_pointers->m_transact_queue, transaction, 69420);
		}
	}

	void features::set_rank(int rpvalue)
	{
		if (rpvalue > 0) {
			if (rpvalue > 8000)
			{
				rpvalue = 8000;
			}

			STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CHAR_SET_RP_GIFT_ADMIN"), leveltable[rpvalue], TRUE);
			STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CHAR_SET_RP_GIFT_ADMIN"), leveltable[rpvalue], TRUE);
		}
	}

	void features::set_crew_rank(int rpvalue)
	{
		if (rpvalue > 0) {
			if (rpvalue > 1000)
			{
				rpvalue = 1000;
			}

			STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_0"), leveltable[rpvalue], TRUE);
			STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_1"), leveltable[rpvalue], TRUE);
			STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_2"), leveltable[rpvalue], TRUE);
			STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_3"), leveltable[rpvalue], TRUE);
			STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_CREW_LOCAL_XP_4"), leveltable[rpvalue], TRUE);
			script::get_current()->yield(100ms);
			STATS::STAT_SAVE(0, 0, 3, 0);
		}
	}

	void features::run_tick()
	{
		ULONGLONG now = GetTickCount64();

		g_local_player = gta_util::get_local_ped();

		if (g_player_info.player_id != PLAYER::PLAYER_ID() || g_player_info.player_id != PLAYER::PLAYER_PED_ID())
		{
			g_player_info.player_id = PLAYER::PLAYER_ID();
			g_player_info.player_ped = PLAYER::PLAYER_PED_ID();
		}
		if (godmode)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), godmode);
		}
		else
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
		}
		
		if (stoptime)
		{
			CLOCK::PAUSE_CLOCK(stoptime);
		}
		if (mobileradio)
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(mobileradio);
		}
		else
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(0);
		}
		if (nophone)
		{
			//PAD::DISABLE_CONTROL_ACTION(0, 27, true);
			
			*script_global(21285).as<int*>() = 6;
			*script_global(19954 + 1).as<int*>() = 3;
		}
		if (superrunbool)
		{
			notifyMap("Hold SHIFT To Activate");
			if (gta_util::IsKeyPressed(VK_SHIFT))
			{
				PED::SET_PED_MOVE_RATE_OVERRIDE(PLAYER::PLAYER_PED_ID(), runspeed);
			}
		}
		if (superjumpbool)
		{
			Player player = PLAYER::PLAYER_ID();
			MISC::SET_SUPER_JUMP_THIS_FRAME(player);
			Ped playerPed = PLAYER::PLAYER_PED_ID();
		}
		if (ultrajumpbool)
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_PED_ID());
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), true, 0, 0, 150, 0, 0, true, true, true, true, false, true, false);
			}
		}
		if (ultrarunbool)
		{
			if (gta_util::IsKeyPressed(VK_SHIFT))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.0f, runspeed1, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
			}
		}
		if (flashrun)
		{
			PED::SET_PED_MOVE_RATE_OVERRIDE(PLAYER::PLAYER_PED_ID(), 3.0);
			GRAPHICS::SET_TIMECYCLE_MODIFIER("RaceTurboFlash");
			if (gta_util::IsKeyPressed(VK_SHIFT))
			{

				MISC::SET_TIME_SCALE(0.5);
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
				//GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_trevor1");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
			}
			else
			{
				MISC::SET_TIME_SCALE(1);
			}
		}
		else
		{
			GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
			MISC::SET_TIME_SCALE(1);
		}
		if (unlimitedstamina)
		{
			PLAYER::GET_PLAYER_SPRINT_STAMINA_REMAINING(PLAYER::PLAYER_PED_ID());
			PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_PED_ID(), 100);
		}
		if (smoothhornboost)
		{
			if (PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_ID()))
			{
				Vehicle Veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false);
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Veh);
				if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Veh))
				{
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(Veh, ENTITY::GET_ENTITY_SPEED(Veh) + 1);
					if (hornboosteffect)
					{
						//GRAPHICS::_START_SCREEN_EFFECT("RaceTurbo", 0, 0);
					}
				}
			}
		}
		if (hornboost)
		{
			if (PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_ID()))
			{
				Vehicle Veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false);
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Veh);
				if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Veh))
				{
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(Veh, hornboostvalue);
					if (hornboosteffect)
					{
						//GRAPHICS::_START_SCREEN_EFFECT("RaceTurbo", 0, 0);
					}
				}
			}
		}
		if (infiniteammo)
		{
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), true);
		}
		else
		{
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), false);
		}
		if (sessioninfo)
		{

			int ConnectedPlayers = (NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS());
			char ConnectedPlayersInfo[60];
			snprintf(ConnectedPlayersInfo, sizeof(ConnectedPlayersInfo), "Connected Players: ~r~ %d", ConnectedPlayers);
			setupdraw();
			int SessionHost = (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0));
			const char* sessionhostname = (PLAYER::GET_PLAYER_NAME(SessionHost));
			char SessionHostInfo[60];
			snprintf(SessionHostInfo, sizeof(SessionHostInfo), "Session Host: %d", sessionhostname);
			setupdraw();
			int Freeslots = (33 - NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS());
			char FreeSlotsInfo[60];
			snprintf(FreeSlotsInfo, sizeof(FreeSlotsInfo), "Free Slots: %d", Freeslots);
			setupdraw();
			int currenttime = time(NULL);
			char CurrentTimeInfo[60];
			int CurrentposX = (ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true).x);
			int CurrentposY = (ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true).y);
			int CurrentposZ = (ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true).z);
			char CurrentposXinfo[60];
			char CurrentposYinfo[60];
			char CurrentposZinfo[60];
			snprintf(CurrentposXinfo, sizeof(CurrentposXinfo), "X: %d", CurrentposX);
			snprintf(CurrentposYinfo, sizeof(CurrentposYinfo), "Y: %d", CurrentposY);
			snprintf(CurrentposZinfo, sizeof(CurrentposZinfo), "Z: %d", CurrentposZ);
			setupdraw();
			snprintf(CurrentTimeInfo, sizeof(CurrentTimeInfo), "%d", currenttime);
			drawstring(ConnectedPlayersInfo, SessionInfoX, 0.070f);
			drawstring(SessionHostInfo, SessionInfoX, 0.095f);
			drawstring(FreeSlotsInfo, SessionInfoX, 0.120f);
			drawstring(CurrentTimeInfo, SessionInfoX, 0.145f);
			drawstring(CurrentposXinfo, SessionInfoX, 0.170f);
			drawstring(CurrentposYinfo, SessionInfoX, 0.195f);
			drawstring(CurrentposZinfo, SessionInfoX, 0.220f);
		}
		if (speedometer)
		{

			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Ped pedSpeed = ENTITY::GET_ENTITY_SPEED(playerPed) * 3.6;
			char EntitySpeedChar[60];
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
			{
				snprintf(EntitySpeedChar, sizeof(EntitySpeedChar), "Your Speed %d KM/H", pedSpeed);
				drawstring(EntitySpeedChar, 0.55f, 0.65f);
			}
		}

		if (spectateplayer) {
			Ped SelectedCoords = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
			//GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.25, 0.25, 0.25, 200, 94, 100, 255, 1, 1, 1, 0, 0, 0, 0);
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(NULL, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL));
		}
		else {
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, PLAYER::PLAYER_PED_ID());
		}

		for (int i = 0; i < sizeof(tick_conf) / sizeof(ULONGLONG); i++)
		{
			if (now - ticks[i] > tick_conf[i])
			{
				g_current_tick_job = i;

				switch (g_current_tick_job)
				{
				case 0:
					//50ms
					break;
				case 1:
					//250ms
					
					break;
				case 2:
					//2000ms
					break;
				case 3:
					//25003ms
					break;
				}

				ticks[i] = now;
			}
		}
	}

	void features::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				if (!g_first)
				{
					g_first = true;
				}
				run_tick();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}
}
