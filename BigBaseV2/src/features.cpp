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
	
	void features::clearwanted()
	{
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID());
	}
	void features::resetped()
	{
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
		PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());

	}
	void features::suicide()
	{
		ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, 0);
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
			int blipIterator = HUD::IS_WAYPOINT_ACTIVE() ? HUD::_GET_BLIP_INFO_ID_ITERATOR() : 1;
			//This is for the race 
			for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator);
				HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator)) {
				if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4 && HUD::GET_BLIP_COLOUR(i) == 5 /* != ColorBlue*/
					&& HUD::IS_BLIP_ON_MINIMAP(i) == 1) {
					wayp = HUD::GET_BLIP_INFO_ID_COORD(i);
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
		//if (!blipFound) {
		//	Blip i = HUD::GET_FIRST_BLIP_INFO_ID(SpriteRaceFinish);
		//	if (HUD::DOES_BLIP_EXIST(i) != 0) {
		//		wayp = HUD::GET_BLIP_INFO_ID_COORD(i);
		//		blipFound = true;
		//		notifyMap("color is: %d, type is: %d",
		//			HUD::GET_BLIP_COLOUR(i), UI::GET_BLIP_INFO_ID_TYPE(i));
		//	}
		//}
		////blipFound ? teleport_to_coords(e, wayp) : NULL;
		//if (teleport_with_cam) {
		//	blipFound ? Features::TPtoWithCam(wayp) : NULL;
		//}
		//else {
		//	blipFound ? teleport_to_coords(e, wayp) : NULL;
		//}
	}
	
	void features::maxvehicle(int VehicleHandle)
	{
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(VehicleHandle, "Zyko");
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(VehicleHandle, 0.0f);
		VEHICLE::SET_VEHICLE_MOD_KIT(VehicleHandle, 0);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(VehicleHandle, 5);
		VEHICLE::TOGGLE_VEHICLE_MOD(VehicleHandle, 18, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(VehicleHandle, 22, 1);
		VEHICLE::TOGGLE_VEHICLE_MOD(VehicleHandle, 20, 1);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 16, 5, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 12, 2, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 11, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 14, 57, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 15, 3, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 13, 2, 0);
		VEHICLE::SET_VEHICLE_WHEEL_TYPE(VehicleHandle, 6);
		VEHICLE::SET_VEHICLE_WINDOW_TINT(VehicleHandle, 5);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 23, 19, 1);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 0, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 1, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 2, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 3, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 4, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 5, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 6, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 7, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 8, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 9, 1, 0);
		VEHICLE::SET_VEHICLE_MOD(VehicleHandle, 10, 1, 0);
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
	void features::spawn_veh(Hash vehicle)
	{
		Hash hash = vehicle;
		while (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}

		auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
		auto veh = VEHICLE::CREATE_VEHICLE(hash, pos.x + 5, pos.y + 5, pos.z, 0.f, TRUE, FALSE, FALSE);
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;



		script::get_current()->yield();
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		if (*g_pointers->m_is_session_started)
		{
			DECORATOR::DECOR_SET_INT(veh, xorstr_("MPBitset"), 0);
			ENTITY::_SET_ENTITY_SOMETHING(veh, TRUE);
			auto networkId = NETWORK::VEH_TO_NET(veh);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
			VEHICLE::SET_VEHICLE_IS_STOLEN(veh, FALSE);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "Zyko");
		}

		if (features::in_vehicle) {
			if (VEHICLE::IS_THIS_MODEL_A_HELI(hash) || VEHICLE::IS_THIS_MODEL_A_PLANE(hash))
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
			VEHICLE::SET_HELI_BLADES_FULL_SPEED(veh);

			if (VEHICLE::IS_THIS_MODEL_A_PLANE(hash))
			{
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
				VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(veh, true);
				Vector3 position = (ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true));
				teleport(position.x, position.y, position.z + 50);

			}
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
			Entity entity = (PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));
			ENTITY::SET_ENTITY_MAX_SPEED(entity, 5000);
		}
		else
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0.0f);

		if (features::full_stats) {
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
			for (int i = 0; i < 999; i++)
			{
				if (i > 16 && i < 23)
					continue;

				VEHICLE::SET_VEHICLE_MOD(veh, i, VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1, false);
			}

		}

		if (features::vehicle_godmode) {
			ENTITY::SET_ENTITY_INVINCIBLE(veh, true);
			ENTITY::SET_ENTITY_PROOFS(veh, true, true, true, true, true, true, true, true);
			VEHICLE::SET_DISABLE_VEHICLE_PETROL_TANK_DAMAGE(veh, true);
			VEHICLE::SET_DISABLE_VEHICLE_PETROL_TANK_FIRES(veh, true);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, false);
			VEHICLE::SET_VEHICLE_CAN_BREAK(veh, false);
			VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(veh, false);
			VEHICLE::SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(veh, false);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, false);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, false);
		}

		//if (strong) {
		//	VEHICLE::SET_VEHICLE_STRONG(veh, true);
		//}


		if (features::vehicle_blip) {
			Hash Model = ENTITY::GET_ENTITY_MODEL(veh);
			Blip last = HUD::GET_BLIP_FROM_ENTITY(veh);
			HUD::REMOVE_BLIP(&last);
			Blip vBlip = HUD::ADD_BLIP_FOR_ENTITY(veh);
			HUD::SET_BLIP_SCALE(vBlip, 1);
			if (VEHICLE::IS_THIS_MODEL_A_CAR(Model))
				HUD::SET_BLIP_SPRITE(vBlip, 225);
			else if (VEHICLE::IS_THIS_MODEL_A_HELI(Model))
				HUD::SET_BLIP_SPRITE(vBlip, 64);
			else if (VEHICLE::IS_THIS_MODEL_A_PLANE(Model))
				HUD::SET_BLIP_SPRITE(vBlip, 16);
			else if (VEHICLE::IS_THIS_MODEL_A_BIKE(Model))
				HUD::SET_BLIP_SPRITE(vBlip, 226);
			else
				HUD::SET_BLIP_SPRITE(vBlip, 225);

			HUD::SET_BLIP_NAME_FROM_TEXT_FILE(vBlip, xorstr_("Pinned Vehicle"));
			HUD::SET_BLIP_COLOUR(vBlip, 0);
			HUD::SET_BLIP_FLASHES(vBlip, false);
			HUD::SET_BLIP_DISPLAY(vBlip, 2);
			HUD::SET_BLIP_CATEGORY(vBlip, 1);
			HUD::BEGIN_TEXT_COMMAND_SET_BLIP_NAME(xorstr_("Alliance's Vehicle"));
			HUD::END_TEXT_COMMAND_SET_BLIP_NAME(vBlip);
		}
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

	void features::startupmoney()
	{
		*script_global(1963962).as<int*>() = 1;
	}

	int delayedPlanned = 400;
	int timerPlaned = 3;


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
	void features::GiveAllWeapons()
	{
	int Weapons[] = { 0x181410A1, 0xC78D71B4, 0x45CD9CF3, 0xDB26713A, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, 0x3813FC08,
								0xA2719263, 0x8BB05FD7, 0xF9DCBF2D, 0xD8DF3C3C, 0xDD5DF8D9, 0xDFE37640, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, 0xBFE256D4, 0x88374054, 0x83839C4, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0x2BE6766B, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x555AF99A, 0xEF951FBB, 0x12E82D3D, 0x394F415C, 0xFAD1F1C9, 0x969C3D67, 0x84D6FAFD, 0x624FE830, 0xDBBD7280, 0xA914799, 0x6A6C02E0, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xA0973D5E, 0xAB564B93, 0xBA45E8B8, 0xFBAB5776, 0x060EC506, 0xAF3696A1, 0xD7DBF707, 0x476BF155, 0xB62D1F67,0xF38A0747, 0x167C5572, 0x2C9CA024, 0xA991DAE8, 0xD6678401, 0x6AA85572
		};
		for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
		}
	}
	void features::RemoveAllWeapons(){WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), true);}
	void features::MaxAmmo(){
	Hash Ammo;	WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &Ammo, 1);
	WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), Ammo, 9999, 9999);
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

		/*if (pedsesp)
		{
			Player playerPed = PLAYER::PLAYER_PED_ID();
			for (PED::GET_PED_NEARBY_PEDS(playerPed, 150, 0)
			{

			}
		}*/


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
		
		if (deletegun)
		{
			Entity delentity;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &delentity)) {
				if (ENTITY::DOES_ENTITY_EXIST(delentity) && NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(delentity)) {
					if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID())) {
						ENTITY::DETACH_ENTITY(delentity, 1, 1);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(delentity, 0, 0, 0, 0, 0, 0);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(delentity, 0, 1);
						ENTITY::DELETE_ENTITY(&delentity);
					}
				}
			}
		}
		
		
		if (features::exploammo)
		{
			Vector3 iCoord;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
			{
				FIRE::ADD_EXPLOSION(iCoord.x, iCoord.y, iCoord.z, 25, 10000.0f, true, false, 0, false);
			}
		}
		if (teleportgun)
		{
			Vector3 iCoord;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
			{

				PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), iCoord.x, iCoord.y, iCoord.z);
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
					
					if (features::vehgodmode)
					{
						Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
						if(PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
						return;
						
							
						
						ENTITY::SET_ENTITY_INVINCIBLE(veh, true);
						ENTITY::SET_ENTITY_PROOFS(veh, true, true, true, true, true, true, true, true);
						VEHICLE::SET_DISABLE_VEHICLE_PETROL_TANK_DAMAGE(veh, true);
						VEHICLE::SET_DISABLE_VEHICLE_PETROL_TANK_FIRES(veh, true);
						VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, false);
						VEHICLE::SET_VEHICLE_CAN_BREAK(veh, false);
						VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(veh, false);
						VEHICLE::SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(veh, false);
						VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, false);
						VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, false);
					}

				
					
					
					if (speedbypass)
					{
						Entity entity = (PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));
						ENTITY::SET_ENTITY_MAX_SPEED(entity, 5000);
						ENTITY::SET_ENTITY_MAX_SPEED(PLAYER::PLAYER_PED_ID(), 5000);
					}
					/*else
					{
						Entity entity = (PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));
						ENTITY::SET_ENTITY_MAX_SPEED(entity, 540);
					}*/
					
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
									GRAPHICS::_START_SCREEN_EFFECT("RaceTurbo", 0, 0);
								}
							}
						}

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
									if (PLAYER::IS_PLAYER_PRESSING_HORN(PLAYER::PLAYER_ID()))
									{
										GRAPHICS::_START_SCREEN_EFFECT("RaceTurbo", 1, false);
									}
								}
							}
						}
					}
					
					if (seatbelt)
					{
						PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 1);
					}
					
					CAM::SET_CAM_FOV(1, features::fieldofview);
					break;
				case 1:
					////////////////////////////////////////   250ms   ////////////////////////////////////////
					
					

					





				


					
					if (fixloop)
					{
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
						VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
					}
					if (cleanloop)
					{
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())), 0);
					}
					

					break;
				case 2:
					//2000ms
					/*if (InternetGetConnectedState(nullptr, 0))
					{
						notify("Nigger", "This is a nigger", 7000);
					}*/


					break;
				case 3:
					//25003ms
					
					
					if (rockstaradmin){features::admindetection();}
					

					if (infiniteammo)
					{
						WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), true);
					}
					else
					{
						WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), false);
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
