#include "common.hpp"
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
#include "fiber_pool.hpp"


namespace big
{

	void features::on_present()
	{
		TRY_CLAUSE
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.04f, 0.14f, 100.f / 255.f)); // Background color
			ImGui::RenderNotifications(); // <-- Here we render all notifications
			ImGui::PopStyleVar(1); // Don't forget to Pop()
			ImGui::PopStyleColor(1);
		}
			EXCEPT_CLAUSE
	}


	
	///////////////////////////////////////////////////////   HELP VOIDS   ///////////////////////////////////////////////////////

	void features::notify_success(const char* text, const char* title, int duration)
	{
		ImGuiToast toast(ImGuiToastType_Success, duration);

		toast.set_title(text);
		toast.set_content(title);

		ImGui::InsertNotification(toast);

	}
	void features::notify(const char* title, const char* text, int duration)
	{
		ImGuiToast toast(ImGuiToastType_Info, duration);

		toast.set_title(title);
		toast.set_content(text);

		ImGui::InsertNotification(toast);

	}
	void features::notify_error(const char* title, const char* text, int duration)
	{
		ImGuiToast toast(ImGuiToastType_Error, duration);

		toast.set_title(title);
		toast.set_content(text);

		ImGui::InsertNotification(toast);

	}
	void features::notify_protections(const char* title, const char* text, int duration)
	{
		ImGuiToast toast(ImGuiToastType_Warning, duration);

		toast.set_title(title);
		toast.set_content(text);

		ImGui::InsertNotification(toast);

	}
	//void features::notify_error(const char* text)
	//{
	//	ImGui::InsertNotification({ ImGuiToastType_Error, 4000, text });
	//}
	//void features::notify_warning(const char* text)
	//{
	//	ImGui::InsertNotification({ ImGuiToastType_Warning, 4000, text });
	//}


	void drawstring(char* text, float X, float Y)
	{

		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::SET_TEXT_FONT(0);
		HUD::SET_TEXT_SCALE(0.3, 0.3f);
		HUD::SET_TEXT_COLOUR(255, 255, 255, 255);
		HUD::SET_TEXT_WRAP(0.0f, 1.0f);
		HUD::SET_TEXT_CENTRE(0);
		HUD::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
		HUD::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		HUD::SET_TEXT_OUTLINE();

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
	void features::RequestControlOfEnt(Entity entity)
	{
		int tick = 0;
		while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
			tick++;
		}
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);

			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
		}
	}
	int pressedKey() {
		int retKey = -1;
		for (int i = 0x00; i < 0xFF; i++) {
			if (gta_util::IsKeyPressed(i) && i != 0x00 && i != VK_NUMPAD5 && i != 0xD) {
				retKey = i;
			}
		}
		return retKey;
	}
	
	
	void features::setOpenKey() {

		int key = pressedKey();
		while (key == -1) {
			key = pressedKey();

		}
		features::OpenKey = key;
	}


	void features::ApplyForceToEntity(Entity e, float x, float y, float z)
	{
		if (e != PLAYER::PLAYER_PED_ID() && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(e) == FALSE)
		{
			features::RequestControlOfEnt(e);
		}

		ENTITY::APPLY_FORCE_TO_ENTITY(e, 1, x, y, z, 0, 0, 0, 0, 1, 1, 1, 0, 1);

	}//APPLIES FORCE TO ENTITY
	void features::teleport(int x, int y, int z)
	{
		PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), x, y, z);
	}
	float degToRad(float degs)
	{
		return degs * 3.141592653589793f / 180.f;
	}
	void features::duplicatecar()
	{
		Vehicle handle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
		const char* vehicle = HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(handle));

		features::spawn_veh(rage::joaat(vehicle));
	}
	
	
	void features::teleport_to_objective()
	{
		Entity e;
		Vector3 wayp;
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE))
			e = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		else e = playerPed;
		bool blipFound = false;
		if (ENTITY::IS_ENTITY_A_VEHICLE(e)) RequestControlOfEnt(e);
		for (int i = 0; i <= 1000; i++)
		{
			int blipIterator = HUD::IS_WAYPOINT_ACTIVE() ? HUD::_GET_WAYPOINT_BLIP_SPRITE() : 1;
			//This is for the race 
			for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator);
				HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator)) {
				if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4 && HUD::GET_BLIP_COLOUR(i) == 5 /* != ColorBlue*/
					&& HUD::IS_BLIP_ON_MINIMAP(i) == 1) {
					Vector3 wayp = HUD::GET_BLIP_INFO_ID_COORD(i);
					blipFound = true;
					//notifyMap("color is: %d, type is: %d", UI::GET_BLIP_COLOUR(i), UI::GET_BLIP_INFO_ID_TYPE(i));
					PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), wayp.x, wayp.y, wayp.z);
				}
				bool groundFound = false;
				static float groundCheckHeight[] =
				{ 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
				for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, wayp.x, wayp.y, groundCheckHeight[i], 0, 0, 1);
					SYSTEM::WAIT(1);
					if (MISC::GET_GROUND_Z_FOR_3D_COORD(wayp.x, wayp.y, groundCheckHeight[i], &wayp.z, 0, 0))
					{
						groundFound = true;
						wayp.z += 3.0;
						break;
					}
				}
				// if ground not found then set Z in air and give player a parachute
				if (!groundFound)
				{
					wayp.z = 1000.0;
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
				}
			}
			break;
		}
		if (!blipFound) {
			Blip i = HUD::GET_FIRST_BLIP_INFO_ID(38);
			if (HUD::DOES_BLIP_EXIST(i) != 0) {
				wayp = HUD::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				//notifyMap("color is: %d, type is: %d",
				HUD::GET_BLIP_COLOUR(i), HUD::GET_BLIP_INFO_ID_TYPE(i);
			}
		}
		blipFound ? PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), wayp.x, wayp.y, wayp.z) : NULL;
		
			
		
	}
	
	




	///////////////////////////////////////////////////////   SPAWN VOIDS   ///////////////////////////////////////////////////////
	void features::spawn_obj(const char* object)
	{
		const char* obj = (const char*)object;
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY(obj), pos.x, pos.y, pos.z, 1, 1, 1);
	}
	void features::play_particle(const char* particle)
	{
		//GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL(particle);
		//GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY(particle, PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);

	}
	

	///////////////////////////////////////////////////////   RECOVERY VOIDS   ///////////////////////////////////////////////////////
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
	

	int delayedPlanned = 400;
	int timerPlaned = 3;


	
	void features::changemodel(const char* model)
	{
		DWORD model1 = MISC::GET_HASH_KEY(model);
		STREAMING::REQUEST_MODEL(model1);
		//while (!STREAMING::HAS_MODEL_LOADED(rage::joaat(model)) /*MISC::WAIT(0);*/
		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), (model1));
		/*PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());*/
		/*WAIT(10);*/
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model1);
	}
	int moddervalue;
	bool features::is_modder(Player player)
	{
		if (auto plyr = g_pointers->m_get_net_player(player))
		{
			if (plyr->m_player_info->m_ped->m_god == 0x01 && !INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player)))
			{
				return true;
			}


		}

		return false;
	}

	/*ATTACK FEATURES*/
	void features::StealWeapons(){WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1);}

	void features::kick(int player)
	{
		//unsigned int player = (1 << features::g_selected_player);

		uint64_t kick1[] = { -371781708, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick2[] = { 1514515570, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick3[] = { 911179316, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick4[] = { 846342319, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick5[] = { 2085853000, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick6[] = { 1514515570, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick7[] = { -1970125962, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), -1, -1, -1 };
		uint64_t kick8[] = { -1013679841, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick9[] = { -1767058336, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), -1, -1, -1 };
		uint64_t kick10[] = { -1892343528, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick11[] = { 1494472464, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick12[] = { 296518236, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t kick13[] = { 998716537, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0 };
		uint64_t superkick[] = { *kick1, *kick2, *kick3, *kick4, *kick5, *kick6, *kick7, *kick8, *kick9, *kick10, *kick11, *kick12, *kick13 };
		g_pointers->m_TriggerScriptEvent(1, superkick, 4, player);

	}
	void features::vehkick(int player)
	{
		uint64_t event[] = { 578856274,PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player) };
		g_pointers->m_TriggerScriptEvent(1, event, 4, player);
	}
	void features::notification(int player)
	{
		//unsigned int player = (1 << features::g_selected_player);
		uint64_t notification[] = { 802133775, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 21000, 21000, 21000 };
		//g_pointers->m_TriggerScriptEvent(1, notification, 4, player);
	}
	void features::crash(int player)
	{
		//unsigned int player = (1 << features::g_selected_player);
		int nigger = rand() % -2147483647 + 2147483647;
		uint64_t crash1[] = { -393294520, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), nigger, nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger };
		uint64_t crash2[] = { -1386010354, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), nigger, nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger };
		uint64_t crash3[] = { 962740265, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), nigger, nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger,nigger };
		uint64_t crash[] = { *crash1, *crash2, *crash3 };
		g_pointers->m_TriggerScriptEvent(1, crash, 4, player);
	}
	void features::kill(int player)
	{

		ENTITY::SET_ENTITY_HEALTH(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0);
	}
	void features::infloading(int player)
	{
		//unsigned int player = (1 << features::g_selected_player);

		uint64_t event[] = { 603406648, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 99, 69 };
		uint64_t event2[] = { 962740265, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0, 0, 32, 32, 32 };
		uint64_t loadingscr[] = { *event, *event2 };
		g_pointers->m_TriggerScriptEvent(1, loadingscr, 4, player);
	}
	void features::transactionerror(int player)
	{
		//unsigned int player = (1 << features::g_selected_player);

		uint64_t event[] = { -1704141512 };
		g_pointers->m_TriggerScriptEvent(1, event, 4, player);
	}

	
	
	void features::change_session(ChangeSessionID ID)
	{
		*script_global(SessionUNK1).as<int*>() = 0;
		*script_global(SessionType).as<int*>() = ID;
		*script_global(InitSession).at(2).as<int*>() = 0;
		*script_global(SessionSCTV).as<int*>() = 0;
		*script_global(InitSession).as<int*>() = 1;
		*script_global(SessionUNK2).as<int*>() = 0;
		*script_global(SessionUNK3).as<int*>() = -1;
	}
	void features::coordsDisplay()
	{

		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		LOG(INFO) << coords.x;
		LOG(INFO) << coords.y;
		LOG(INFO) << coords.z;
	}
	
	void features::teleport_to_waypoint()
	{
		
			Vector3 coords;
			Entity e = PLAYER::PLAYER_PED_ID();
			if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
				e = PED::GET_VEHICLE_PED_IS_USING(e);

			int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8);
			if (HUD::DOES_BLIP_EXIST(WaypointHandle))
			{
				Vector3 WaypointPos = HUD::GET_BLIP_COORDS(WaypointHandle);
				WaypointPos.z += 5.0f;
				int Handle = PLAYER::PLAYER_PED_ID();
				if (PED::IS_PED_IN_ANY_VEHICLE(Handle, 0))
					Handle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
				ENTITY::SET_ENTITY_COORDS(Handle, WaypointPos.x, WaypointPos.y, WaypointPos.z, 0, 0, 0, 1);
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

		
		
		if (features::novehkick)
		{
			*script_global(2689224).at(PLAYER::PLAYER_PED_ID()).at(317).at(10).as<int*>() = 21501;
			*script_global(1958845).as<int*>() = 1;
			script_global(262145).at(7478);

			//Global_262145.f_7478
		}

		

		
		if (gta_util::IsKeyPressed(VK_F5))
		{
			features::teleport_to_waypoint();
		}

		if (playeresp)
		{

			Player playerPed = PLAYER::PLAYER_PED_ID();
			for (int i = 0; i < 32; i++)
			{
				Player playerHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
				Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
				const char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

				if (ENTITY::DOES_ENTITY_EXIST(playerHandle))
				{
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
					Vector3 locationOne = ENTITY::GET_ENTITY_COORDS(playerHandle, false);
					Vector3 locationTwo = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
					GRAPHICS::DRAW_LINE(locationOne.x, locationOne.y, locationOne.z, locationTwo.x, locationTwo.y, locationTwo.z, 255, 0, 0, 255);

				}
			}
		}
		for (int i = 0; i < sizeof(tick_conf) / sizeof(ULONGLONG); i++)
		{
			if (now - ticks[i] > tick_conf[i])
			{
				g_current_tick_job = i;

				switch (g_current_tick_job)
				{
				case 0:
					//50 MS				
					run_playerlist();
					features::player_loop();
					features::online_loop();
					features::weapons_loop();
					features::vehicle_loop();
	
					CAM::SET_CAM_FOV(1, features::fieldofview);
					break;
				case 1:
					////////////////////////////////////////   250ms   ////////////////////////////////////////
					
					

					





				


					
					
					

					break;
				case 2:
					


					break;
				case 3:
					//25003ms
					
					
					if (rockstaradmin){features::admindetection();}
					

					
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
					

					ticks[i] = now;
				}
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
