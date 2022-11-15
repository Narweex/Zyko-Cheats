#include "features.hpp"
#include "logger.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "helpers/player_info.h"
#include "script_global.hpp"
#include "gta_util.hpp"
#include <imgui.h>
#include "gui/player_list.h"
#include "fiber_pool.hpp"




namespace zyko
{
	void features::on_present()
	{
		TRY_CLAUSE
		{
			show_fps(fps);
			show_watermark(watermark);
			sessioninfo(features::session_info);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f); // Round borders
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.04f, 0.14f, 100.f / 255.f)); // Background color
			ImGui::RenderNotifications(); // <-- Here we render all notifications
			ImGui::PopStyleVar(1); // Don't forget to Pop()
			ImGui::PopStyleColor(1);
		}
			EXCEPT_CLAUSE
	}


	void features::show_fps(bool toggle)
	{
		if (toggle)
		{

			ImGui::SetNextWindowSize(ImVec2(100.f, 40.f), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(13.5f, 250.f), ImGuiCond_Always);
			ImGui::Begin(xorstr_("##fps"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			int fps = ImGui::GetIO().Framerate / 2;
			std::string tostring = (std::to_string(fps));
			ImGui::Text(xorstr_("FPS: %s"), tostring.c_str());
		}

	}

	void features::show_watermark(bool enable)
	{
		auto end = std::chrono::system_clock::now();

		std::time_t end_time = std::chrono::system_clock::to_time_t(end);


		if (enable)
		{
			// Size
			ImVec2 size = ImVec2(300.f, 70.f);

			// Position
			ImGui::SetNextWindowSize(size, ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(14.f, 5.f));

			// Window
			if (ImGui::Begin(xorstr_("##watermark"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse /*| ImGuiWindowFlags_NoNav*/ | ImGuiWindowFlags_NoResize | /*ImGuiWindowFlags_NoFocusOnAppearing |*/ ImGuiWindowFlags_NoBringToFrontOnFocus))
			{
				ImGui::Text(xorstr_("zykocheats.org"));
				ImGui::Text(fmt::format(xorstr_("Time: {}"), std::ctime(&end_time)).c_str());
			}
			ImGui::End();
		}
	}

	void features::sessioninfo(bool toggle)
	{
		if (toggle/* && *g_pointers->m_is_session_started*/)
		{
			ImVec2 size = ImVec2(300.f, 100.f);

			// Position
			ImGui::SetNextWindowSize(size, ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(300.f, 900.f));

			if (ImGui::Begin(xorstr_("##sessionInfo"), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground))
			{
				if (*g_pointers->m_is_session_started)
				{
					static int connected_players = gta_util::get_connected_players();
					
					ImGui::Text(xorstr_("Players: %d"), connected_players); 
					ImGui::Text(xorstr_("Free slots: %d"), 32 - connected_players);
				}
				
				
			}
		}

	}


	void features::show_info_pool(bool enable)
	{
		if (enable)
		{
			// Size
			ImVec2 size = ImVec2(275.f, 55.f);

			// Data
			struct data_s
			{
				std::string text{};
				int amount{};
			};

			std::vector<data_s> pools
			{
				/*{ xorstr_("Peds"), g_pointers->m_ped_pool->size },
				{ xorstr_("Objects"), g_pointers->m_prop_pool->size }*/
			};

			// Dynamic size
			for (const auto& rs : pools)
				size.y += 25.f;

			// Position
			ImGui::SetNextWindowSize(size, ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(13.5f, 90.f), ImGuiCond_Always);

			// Window
			if (ImGui::Begin(xorstr_("##pools"), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoInputs))
			{
				std::string str;

				for (const auto& rs : pools)
				{
					// Text
					str += rs.text + xorstr_(": ") + std::to_string(rs.amount);

					// Newline
					str += xorstr_("\n");
				}

				ImGui::Text(str.c_str());
			}
			ImGui::End();
		}
	}

	///////////////////////////////////////////////////////   HELP VOIDS   ///////////////////////////////////////////////////////

	std::string features::Keyboard()
	{
		MISC::DISPLAY_ONSCREEN_KEYBOARD(1, "Input", "", "", "", "", "", 32);
		
		return MISC::GET_ONSCREEN_KEYBOARD_RESULT();

	}


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
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
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
		if (e != PLAYER::PLAYER_PED_ID() && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(e))
		{
			features::RequestControlOfEnt(e);
		}

		ENTITY::APPLY_FORCE_TO_ENTITY(e, 1, x, y, z, 0, 0, 0, 0, 1, 1, 1, 0, 1);

	}
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
		//Vehicle handle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
		//const char* vehicle = HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(handle));
		//features::notify(vehicle, "Nigger", 7000);
/*
		features::spawn_veh(rage::joaat(vehicle));*/
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








	void features::changemodel(const char* model)
	{
		Hash model1 = MISC::GET_HASH_KEY(model);
		STREAMING::REQUEST_MODEL(model1);
		if (STREAMING::HAS_MODEL_LOADED(model1))
		{
			SYSTEM::WAIT(0);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), (model1));
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);

		}


	}
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
	void features::StealWeapons() { WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1); }

	void features::kick(int player)
	{
		//unsigned int player = (1 << features::g_selected_player);

		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		RequestControlOfEnt(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		TASK::CLEAR_PED_TASKS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		TASK::CLEAR_PED_SECONDARY_TASK(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
		STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("vw_prop_vw_colle_alien"));
		if (STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("vw_prop_vw_colle_alien")))
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 0, 0, 2600, MISC::GET_HASH_KEY("vw_prop_vw_colle_alien"), 1, 1);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(MISC::GET_HASH_KEY("vw_prop_vw_colle_alien"));
		}
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));

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
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), 0);
		OBJECT::CREATE_OBJECT(0x337b2b54, coords.x, coords.y, coords.z, 1, 0, 1);
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

		//g_local_player = gta_util::get_local_ped();

		/*if (g_player_info.player_id != PLAYER::PLAYER_ID() || g_player_info.player_id != PLAYER::PLAYER_PED_ID())
		{
			g_player_info.player_id = PLAYER::PLAYER_ID();
			g_player_info.player_ped = PLAYER::PLAYER_PED_ID();
		}*/
		//if (crosshair)
		//{
		//	CAM::_ENABLE_CROSSHAIR_THIS_FRAME();
		//}

		
		




		//if (gta_util::IsKeyPressed(VK_F5))
		//{
		//	features::teleport_to_waypoint();
		//}

		
	
		//UpdatePresence();

		features::esp_line_all||features::esp_box_all||features::esp_name_all ||features::esp_distance_all ? features::ESP_all() : NULL;

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
					features::shootpeds ? Shootpeds(true) : Shootpeds(false);
					//notify_on_join ? features::joinNotification : NULL;
					features::godmode ? Godmode(true) : Godmode(false);
					neverwanted ? Neverwanted(true) : Neverwanted(false);
					noclip ? Noclip(true) : Noclip(false);
					instantenter ? Instantenter(true) : Instantenter(false);
					superman ? Superman(true) : Superman(false);
					nightvision ? Nightvision(true) : Nightvision(false);
					thermalvision ? Thermalvision(true) : Thermalvision(false);
					flashrun ? Flashrun(true) : Flashrun(false);
					invisibility ? Invisibility(true) : Invisibility(false);
					norag ? Norag(true) : Norag(false);
					semigod ? Semigod(true) : Semigod(false);
					bullshark ? Bullshark(true) : Bullshark(false);
					offradar ? Offradar(true) : Offradar(false);
					mobileradio ? Mobileradio(true) : Mobileradio(false);
					hide_hud ? Hide_hud(true) : Hide_hud(false);
					superjumpbool ? Superjump(true) : Superjump(false);
					ultrajumpbool ? Ultrajumpbool(true) : Ultrajumpbool(false);
					noidlekick ? Noidlekick(true) : Noidlekick(false);
					superrunbool ? Superrun(true) : Superrun(false);
					forcefield ? Forcefield(true) : Forcefield(false);
					unlimitedstamina ? Unlimitedstamina(true) : Unlimitedstamina(false);
					ignoreplayer ? Ignoreplayer(true) : Ignoreplayer(false);
					ultrarunbool ? Ultrarunbool(true) : Ultrarunbool(false);

					driveitgun ? Driveitgun(true) : Driveitgun(false);
					teleportgun ? Teleportgun(true) : Teleportgun(false);
					aimbot ? Aimbot(true) : Aimbot(false);
					deletegun ? Deletegun(true) : Deletegun(false);
					airstrikegun ? Airstrikegun(true) : Airstrikegun(false);
					exploammo ? Exploammo(true) : Exploammo(false);
					infiniteammo ? Infiniteammo(true) : Infiniteammo(false);

					cleanloop ? Cleanloop(true) : Cleanloop(false);
					speedbypass ? Speedbypass(true) : Speedbypass(false);
					novehkick ? Novehkick(true) : Novehkick(false);
					fixloop ? Fixloop(true) : Fixloop(false);
					vehgodmode ? Vehgodmode(true) : Vehgodmode(false);
					hornboost ? Hornboost(true) : Hornboost(false);
					fucktheircam ? Fucktheircam(true) : Fucktheircam(false);
					spectateplayer ? Spectateplayer(true) : Spectateplayer(false);
					ragdoll_player ? Ragdoll_player() : NULL;
					crosshair ? Crosshair(true) : Crosshair(false);
					stoptime ? CLOCK::PAUSE_CLOCK(true) : CLOCK::PAUSE_CLOCK(false);
					rainbowcar ? Rainbowcar(true) : Rainbowcar(false);
					flashrainbow ? Flashrainbow(true) : Flashrainbow(false);
					rotating ? Rotating(true) : Rotating(false);
					invis_car ? Invis_car(true) : Invis_car(false);
					deadeye ? Deadeye(true) : Deadeye(false);

					break;
				case 1:
					////////////////////////////////////////   2500ms   ////////////////////////////////////////
					moneynotify ? Moneynotify(true) : Moneynotify(false);
					
					nophone ? Nophone(true) : Nophone(false);
					
					if (!invisibility) {
						ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), features::playeralpha, false);
					}

					break;
				case 2:
					kick_from_mk2 ? Kick_from_mk2(true) : Kick_from_mk2(false);

					break;
				case 3:
					features::nigger();





					

					


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
				/*if (!g_first)
				{
					g_first = true;
				}*/
				run_tick();
			}
				EXCEPT_CLAUSE
				script::get_current()->yield();
		}
	}
}
