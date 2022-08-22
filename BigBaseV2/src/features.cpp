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
#include "auth/anti_debug.hpp"
#include <WinInet.h>

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
	void RequestControlOfEnt(Entity entity)
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
	}//REQUEST CONTROL OF ENTITY
	//bool features::GetEventData(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount)
	//{
	//	auto result = static_cast<decltype(&GetEventData)>(eventGroup, eventIndex, args, argCount);

	//	if (result && features::g_log_net_event_data)
	//	{
	//		LOG(INFO)<<"Script event group: "<< eventGroup;
	//		LOG(INFO) << "Script event index: " << eventIndex;
	//		LOG(INFO) << "Script event argcount: "<< argCount;
	//		for (std::uint32_t i = 0; i < argCount; ++i)
	//			LOG(INFO) << "Script event args[%u] : %"<< PRIi64, i, args[i];
	//	}

	//	return result;
	//}
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


	void ApplyForceToEntity(Entity e, float x, float y, float z)
	{
		if (e != PLAYER::PLAYER_PED_ID() && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(e) == FALSE)
		{
			RequestControlOfEnt(e);
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
	void big::features::tpobjective()
	{
		if (HUD::DOES_BLIP_EXIST(143 || 144 || 145 || 146))
		{
			Vector3 prdel = HUD::GET_BLIP_COORDS(143 || 144 || 145 || 146);
			PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), prdel.x, prdel.y, prdel.z + 5);
		}

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
	void features::maxhealth() { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 400, 100); }
	void features::maxarmor() { PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200); }
	void features::setwanted() {
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
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
	void features::noIdleKick()
	{
		*script_global(features::Tunables).at(87).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(88).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(89).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(90).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(7785).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(7786).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(7787).as<int*>() = INT_MAX;
		*script_global(features::Tunables).at(7788).as<int*>() = INT_MAX;
		*script_global(features::IdleTimer).at(features::IdleTimer_Offset_1).as<int*>() = 0;
		*script_global(features::IdleTimer).at(features::IdleTimer_Offset_2).as<int*>() = 0;
		*script_global(features::IdleTimerUNK1).as<int*>() = 0;
		*script_global(features::IdleTimerUNK2).as<int*>() = 0;
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
		if (neverWanted)
		{

			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);

		}
		else
		{
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
		}
		if (features::novehkick)
		{
			*script_global(2689224).at(PLAYER::PLAYER_PED_ID()).at(317).at(10).as<int*>() = 21501;
			*script_global(1958845).as<int*>() = 1;
			script_global(262145).at(7478);

			//Global_262145.f_7478
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
		if (superjumpbool)
		{
			Player player = PLAYER::PLAYER_ID();
			MISC::SET_SUPER_JUMP_THIS_FRAME(player);
			Ped playerPed = PLAYER::PLAYER_PED_ID();
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
		if (ultrajumpbool)
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_PED_ID());
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), true, 0, 0, 150, 0, 0, true, true, true, true, false, true, false);
			}
		}
		if (offradar) {

			/**script_global(2689224).at(PLAYER::GET_PLAYER_INDEX(), 451).at(207).as<int*>() = 1;
			*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;*/
			*script_global(2689235).at(PLAYER::GET_PLAYER_INDEX(), 453).at(208).as<int*>() = 1;
			*script_global(2703735).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;
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


		if (modifytimecycle)
		{
			MISC::SET_TIME_SCALE(features::timescale);
		}

		if (driveitgun)
		{

			Vector3 iCoord;
			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
			{
				int vehicle = VEHICLE::GET_CLOSEST_VEHICLE(iCoord.x, iCoord.y, iCoord.z, 15.0f, 0, 1);
				int driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, -1, 0);
				RequestControlOfEnt(vehicle);
				RequestControlOfEnt(driver);
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(driver);
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), vehicle, -1);
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
					if (instartenter)
					{

						Vehicle vehicle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(PLAYER::PLAYER_PED_ID());
						Ped del = VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, -1, 0);
						RequestControlOfEnt(vehicle);
						RequestControlOfEnt(del);
						TASK::CLEAR_PED_TASKS_IMMEDIATELY(del);
						PED::DELETE_PED(&del);
						PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), vehicle, -1);
						PED::SET_PED_VEHICLE_FORCED_SEAT_USAGE(PLAYER::PLAYER_PED_ID(), vehicle, 0, 0);


					}
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

					if (godmode)
					{
						
						/*if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID(), 1) || !ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
							break;
						else
							continue;*/
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), godmode);

					}
					/*else
					{
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
					}*/
					if (features::invisibility)
					{
						ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 0, false);

					}
					else
					{
						ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), features::playeralpha, false);
					}
					if (features::norag)
					{
						PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), !features::norag);
						PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), !features::norag);
						PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), !features::norag);
					}
					if (features::semigod)
					{

					}
					if (features::noclip)
					{

						static const int controls[] = { 21, 32, 33, 34, 35, 36 };
						static float speed = 20.f;
						static float mult = 0.f;

						static bool bLastNoclip = false;

						static Entity prev = -1;
						static Vector3 rot{};


						bool bNoclip = features::noclip;

						Entity ent = PLAYER::PLAYER_PED_ID();
						bool bInVehicle = PED::IS_PED_IN_ANY_VEHICLE(ent, true);
						if (bInVehicle) ent = PED::GET_VEHICLE_PED_IS_IN(ent, false);

						// cleanup when changing entities
						if (prev != ent)
						{
							ENTITY::FREEZE_ENTITY_POSITION(prev, false);
							ENTITY::SET_ENTITY_COLLISION(prev, true, true);

							prev = ent;
						}

						if (bNoclip)
						{
							for (int control : controls)
								PAD::DISABLE_CONTROL_ACTION(0, control, true);

							Vector3 vel = { 0.f, 0.f, 0.f };
							float heading = 0.f;

							// Left Shift
							if (gta_util::IsKeyPressed(VK_SPACE))
								vel.z += speed / 2;
							// Left Control
							if (gta_util::IsKeyPressed(VK_SHIFT))
								vel.z -= speed / 2;
							// Forward
							if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 32))
								vel.y += speed;
							// Backward
							if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 33))
								vel.y -= speed;
							// Left
							if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 34))
								vel.x -= speed;
							// Right
							if (PAD::IS_DISABLED_CONTROL_PRESSED(0, 35))
								vel.x += speed;

							rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
							ENTITY::SET_ENTITY_ROTATION(ent, 0.f, rot.y, rot.z, 2, 0);
							ENTITY::SET_ENTITY_COLLISION(ent, false, false);
							if (vel.x == 0.f && vel.y == 0.f && vel.z == 0.f)
							{
								// freeze entity to prevent drifting when standing still
								ENTITY::FREEZE_ENTITY_POSITION(ent, true);

								mult = 0.f;
							}
							else
							{
								if (mult < 20.f)
									mult += 0.15f;

								ENTITY::FREEZE_ENTITY_POSITION(ent, false);
								Vector3 pos = (ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1));
								Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, vel.x, vel.y, 0.f);
								vel.x = offset.x - pos.x;
								vel.y = offset.y - pos.y;

								ENTITY::SET_ENTITY_VELOCITY(ent, vel.x * mult, vel.y * mult, vel.z * mult);
							}
						}
						else if (bNoclip != bLastNoclip)
						{

							RequestControlOfEnt(PLAYER::PLAYER_PED_ID());
							ENTITY::FREEZE_ENTITY_POSITION(ent, false);
							ENTITY::SET_ENTITY_COLLISION(ent, true, false);

						}

						bLastNoclip = bNoclip;


					}
					else
					{
						ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
						ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), true, false);
						ENTITY::FREEZE_ENTITY_POSITION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true), false);
						ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true), true, false);
					}

					if (superrunbool)
					{

						if (gta_util::IsKeyPressed(VK_SHIFT))
						{
							PED::SET_PED_MOVE_RATE_OVERRIDE(PLAYER::PLAYER_PED_ID(), runspeed);
						}
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
					if (forcefield) {
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), forcefield);
						Player playerPed = PLAYER::PLAYER_PED_ID();
						PED::SET_PED_CAN_RAGDOLL(playerPed, false);
						Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 7, 5.00f, 0, 1, 0, 1);

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
									GRAPHICS::_START_SCREEN_EFFECT("RaceTurbo", 0, 0);
								}
							}
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
							/*GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
							STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
							GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_trev1_trailer_boosh");
							GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
						*/
						}
						else
						{
							//MISC::SET_TIME_SCALE(1);
							GRAPHICS::SET_TIMECYCLE_MODIFIER("li");;
						}
					}
					else
					{

						MISC::SET_TIME_SCALE(1);
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
					if (superman)
					{


						if (gta_util::IsKeyPressed(VK_SPACE))
						{
							ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0, 0, 10, 0, 0, 0, 1, true, true, true, true, true);
						}
						Hash hash = MISC::GET_HASH_KEY("GADGET_PARACHUTE");
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), hash, 1, 1);

						if (ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_RAGDOLL(PLAYER::PLAYER_PED_ID()))
						{
							if (gta_util::IsKeyPressed(0x57)) // W key
							{
								ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 0, 0);
							}

							if (gta_util::IsKeyPressed(0x53)) // S key
							{
								ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 3, 6, 0);
							}
							if (gta_util::IsKeyPressed(VK_SHIFT))
							{
								ApplyForceToEntity(PLAYER::PLAYER_PED_ID(), 6, 0, 0);
							}
						}

					}
					if (seatbelt)
					{
						PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 1);
					}
					if (aimbot)
					{
						int player = PLAYER::PLAYER_ID();
						int playerPed = PLAYER::PLAYER_PED_ID();

						for (int i = 0; i < 32; i++)
						{
							if (i != player)
							{
								if (GetAsyncKeyState(VK_RBUTTON))
								{
									Ped targetPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
									Vector3 targetPos = ENTITY::GET_ENTITY_COORDS(targetPed, 1);
									BOOL exists = ENTITY::DOES_ENTITY_EXIST(targetPed);
									BOOL dead = PLAYER::IS_PLAYER_DEAD(targetPed);

									if (exists && !dead)
									{
										float screenX, screenY;
										BOOL onScreen = GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(targetPos.x, targetPos.y, targetPos.z, &screenX, &screenY);
										if (ENTITY::IS_ENTITY_VISIBLE(targetPed) && onScreen)
										{
											if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, targetPed, 17) && PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
											{
												Vector3 targetCoords = PED::GET_PED_BONE_COORDS(targetPed, 31086, 0, 0, 0);
												PED::SET_PED_SHOOTS_AT_COORD(playerPed, targetCoords.x, targetCoords.y, targetCoords.z, 1);

											}
										}
									}
								}
							}
						}
					}
					CAM::SET_CAM_FOV(1, features::fieldofview);
					break;
				case 1:
					////////////////////////////////////////   250ms   ////////////////////////////////////////
					if (unlimitedstamina)
					{
						PLAYER::GET_PLAYER_SPRINT_STAMINA_REMAINING(PLAYER::PLAYER_PED_ID());
						PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_PED_ID(), 100);
					}
					if (noidlekick) { features::noIdleKick(); }
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

					if (fucktheircam)
					{
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
						FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 4, 0.f, false, true, 10000.f, true);
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


					if (offradar)
					{
						*script_global(2426865).at((PLAYER::PLAYER_PED_ID(), 451)).at(207).as<int*>() = 1;

						*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;


					}
					if (bullshark)
					{
						*script_global(2703660).at(3576).as<int*>() = 1;




					}
					if (fixloop)
					{
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
						VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
					}
					if (cleanloop)
					{
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())), 0);
					}
					if (features::nightvision) {
						GRAPHICS::SET_NIGHTVISION(true);
					}
					else {
						GRAPHICS::SET_NIGHTVISION(false);
					}
					if (features::thermalvision) {
						GRAPHICS::SET_SEETHROUGH(true);
					}
					else {
						GRAPHICS::SET_SEETHROUGH(false);

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
					
					
					
					if (features::notifyadmin)
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
								features::change_session(features::FindPublicSession);

							}
							else if (features::crashgame)
							{
								exit(0);
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
