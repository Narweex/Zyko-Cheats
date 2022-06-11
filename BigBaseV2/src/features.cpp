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
#include "../../BigBaseV2/src/helpers/tahoma.h"


namespace big
{
	void features::on_present()
	{
		

		// Initialize notify
		
		TRY_CLAUSE
		{
		ImGui::MergeIconsWithLatestFont(16.f, false);
		ImGui::MergeIconsWithLatestFont(16.f, false);
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
	void features::notify(const char* title,const char* text, int duration)
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
	
	void features::notifyMap(char* fmt, ...)
	{
		char buf[2048] = { 0 };
		va_list va_alist;

		va_start(va_alist, fmt);
		vsprintf_s(buf, fmt, va_alist);
		va_end(va_alist);

		char buff2[2048] = { 0 };
		sprintf_s(buff2, "%s", buf);

		HUD::SET_TEXT_OUTLINE();
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buff2);
		HUD::_DRAW_NOTIFICATION(FALSE, FALSE);
	}	void notifyMap(std::string str) { notifyMap(&str[0]); }
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

	Hash PlanesToBeAngry[21] = {
		////plane
		//link-https://wiki.rage.mp/index.php?title=Vehicles
		//0xA52F6866/*alphaz1*/,
		0x81BD2ED0/*avenger*/,
		0x18606535/*avenger2*/,
		//0x6CBD1D6D/*besra*/,
		//0xF7004C86/*blimp*/,
		//0xDB6B4924/*blimp2*/,
		//0xEDA4ED97/*blimp3*/,
		0xFE0A508C/*bombushka*/,
		//0x15F27762/*cargoplane*/,
		//0xD9927FE3/*cuban800*/,
		//0xCA495705/*dodo*/,
		//0x39D6779E/*duster*/,
		//0xC3F25753/*howard*/,
		0x39D6E83F/*hydra*/,
		//0x3F119114/*jet*/,
		0xB39B0AE6/*lazer*/,
		//0x250B0C5E/*luxor*/,
		//0xB79F589E/*luxor2*/,
		//0x97E55D11/*mammatus*/,
		//0x96E24857/*microlight*/,
		//0x9D80F93/*miljet*/,
		//0xD35698EF/*mogul*/,
		//0x5D56F01B/*molotok*/,
		//0xB2CF7250/*nimbus*/,
		//0x3DC92356/*nokota*/,
		//0xAD6065C0/*pyro*/,
		//0xC5DD6967/*rogue*/,
		//0xE8983F9F/*seabreeze*/,
		//0xB79C1BF5/*shamal*/,
		//0x9A9EB7DE/*starling*/,
		0x64DE07A1/*strikeforce*/,
		//0x81794C70/*stunt*/,
		//0x761E2AD3/*titan*/,
		0x3E2E4F8A/*tula*/,
		//0x9C429B6A/*velum*/,
		//0x403820E8/*velum2*/,
		//0x4FF77E37/*vestra*/,
		//0x1AAD0DED/*volatol*/,
		////choper
		0x46699F47/*akula*/,
		//0x31F0B376/*annihilator*/,
		0x2F03547B/*buzzard*/,
		0x2C75F0DD/*buzzard2*/,
		0xFCFCB68B/*cargobob*/,
		0x60A7EA10/*cargobob2*/,
		0x53174EEF/*cargobob3*/,
		0x78BC1A3C/*cargobob4*/,
		//0x2C634FBD/*frogger*/,
		//0x742E9AC0/*frogger2*/,
		//0x89BA59F5/*havok*/,
		0xFD707EDE/*hunter*/,
		//0x9D0450CA/*maverick*/,
		0xFB133A17/*savage*/,
		//0xD4AE63D9/*seasparrow*/,
		0x3E48BF23/*skylift*/,
		//0x2A54C47D/*supervolito*/,
		//0x9Cregular644/*supervolito2*/,
		0xEBC24DF2/*swift*/,
		0x4019CB4C/*swift2*/,
		0xA09E15FD/*valkyrie*/,
		0x5BFA5C4B/*valkyrie2*/,
		//0x920016F1/*volatus*/,
	};
	Hash VehsToFollow[21] = {
		0x8E9254FB,
		0xB5FCF74E,
		0x8FB66F9B,
		0xBB6B404F
	};
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
	int moddervalue;
	bool features::is_modder(Player player)
	{
		if (auto plyr = g_pointers->m_get_net_player(player))
		{
			if (plyr->m_player_info->m_ped->m_god == 0x01 && !INTERIOR::GET_INTERIOR_FROM_ENTITY(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player)))
				moddervalue + 5;
			//return true;
			if (ENTITY::GET_ENTITY_SPEED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player)))
			{
				moddervalue + 5;
			}
			if (moddervalue > 9)
			{
				return true;
			}
		}

		return false;
	}

	/*ATTACK FEATURES*/
	void features::crash()
	{
		
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		Vector3 ppos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		Ped ped = PED::CREATE_PED(26, 2727244247, ppos.x, ppos.y, ppos.z, MISC::GET_RANDOM_INT_IN_RANGE(0, 255),0, 0);
			pos.x = pos.x + 5;
		ppos.z = ppos.z + 1;
		Ped pedp = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player);
		Vehicle cargobob = VEHICLE::CREATE_VEHICLE(2132890591, pos.x, pos.y, pos.z, MISC::GET_RANDOM_INT_IN_RANGE(0, 255),0,0, 0);
		Vector3 idk = ENTITY::GET_ENTITY_COORDS(cargobob, true);
		int rope = PHYSICS::ADD_ROPE(pos.x, pos.y, pos.z,0, 0, 0, 1, 1, 0.00000000000000000001, 1, 1, true, true, true, 1.0, 0,0);	
		PHYSICS::ATTACH_ENTITIES_TO_ROPE(rope, cargobob, features::g_selected_player, pos.x, pos.y, pos.z, idk.x, idk.y, idk.z, 2, 0, 0, 0, 0);

		//////script event
		
		
		
		std::uint64_t crash1[] = { (-393294520, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647),MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647)) };
		std::uint64_t crash2[] = { (-1386010354, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player),  PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647)) };
		std::uint64_t crash3[] = { (962740265, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player),  PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647), MISC::GET_RANDOM_INT_IN_RANGE(-2147483647, 2147483647)) };
			

		g_pointers->m_TriggerScriptEvent(1 , crash1, 4, 1);
		g_pointers->m_TriggerScriptEvent(1, crash2, 4, 1);
		g_pointers->m_TriggerScriptEvent(1, crash3, 4, 1);
		

		

	}
	void features::kick()
	{
		unsigned int player = (1 << features::g_selected_player);
		
		uint64_t kick1[] = { -371781708, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick2[] = { 1514515570, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick3[] = { 911179316, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick4[] = { 846342319, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick5[] = { 2085853000, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick6[] = { 1514515570, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick7[] = { -1970125962, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), -1, -1, -1 };
		uint64_t kick8[] = { -1013679841, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick9[] = { -1767058336, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), -1, -1, -1 };
		uint64_t kick10[] = { -1892343528, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick11[] = { 1494472464, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick12[] = { 296518236, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		uint64_t kick13[] = { 998716537, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0 };
		g_pointers->m_TriggerScriptEvent(1, kick1, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick2, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick3, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick4, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick5, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick6, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick7, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick8, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick9, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick10, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick11, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick12, 4, player);
		g_pointers->m_TriggerScriptEvent(1, kick13, 4, player);
	}
	void features::notification()
	{
		unsigned int player = (1 << features::g_selected_player);
		uint64_t notification[] = { 802133775, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 21000, 21000, 21000 };
		g_pointers->m_TriggerScriptEvent(1, notification, 4, player);
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
		if (croshair)
		{
			CAM::_ENABLE_CROSSHAIR_THIS_FRAME();
		}
		Entity detentity = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player);
		Vector3 myCoord = ENTITY::GET_ENTITY_COORDS(detentity, true);
		Vector3 speed = ENTITY::GET_ENTITY_SPEED_VECTOR(detentity, 1);
		const int ElementAmount = 31;
		const int ArrSize = ElementAmount * 2 + 2;
		Vehicle* vehs = new Vehicle[ArrSize];
		vehs[0] = ElementAmount;
		if (angryplanesonplayer)
		{
			//PLAYER::_EXPAND_WORLD_LIMITS(FLT_MAX, FLT_MAX, FLT_MAX);
			
				
					/*Vector3 wpVec = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
					float heading = (ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0)).z;
					FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
					Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);*/
					Vector3 wpVec = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1);
					float heading = (ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0)).z;
					FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
					Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1);
					if (MISC::GET_GAME_TIMER() > timerPlaned)
					{
						if (STREAMING::IS_MODEL_IN_CDIMAGE(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)]) && STREAMING::IS_MODEL_A_VEHICLE(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)]))
						{
							STREAMING::REQUEST_MODEL(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)]);
							//while (!STREAMING::HAS_MODEL_LOADED(PlanesToBeAngry[GAMEPLAY::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)]))
							//{
							//	/*make_periodic_feature_call();*/
							//	/*WAIT(0)*/;	
							//}
							if (STREAMING::HAS_MODEL_LOADED(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)]))
							{
								Vector3 Coordsx;
								Coordsx.x = coords.x + rand() % 339;
								Coordsx.y = coords.y + rand() % 669;
								Coordsx.z = coords.z + rand() % 29 + 69;
								int SpawnedPlanes = VEHICLE::CREATE_VEHICLE(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*/25)], coords.x + rand() % 339, coords.y + rand() % 669, coords.z + rand() % 29 + 69, lookDir, 1, 1, 1);
								//int SpawnedPlanes = CHooking::create_vehicle(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)], &Coordsx, lookDir, 1, 1);
								ENTITY::SET_ENTITY_INVINCIBLE(SpawnedPlanes, 1);
								Ped planePilot = PED::CREATE_RANDOM_PED_AS_DRIVER(SpawnedPlanes, false);
								PED::SET_DRIVER_ABILITY(planePilot, 1000.0f);
								PED::SET_DRIVER_AGGRESSIVENESS(planePilot, 1000.0f);
								/*PED::SET_PED_COMBAT_ABILITY(planePilot, 100);
								AI::TASK_COMBAT_PED(planePilot, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 1, 1);
								PED::SET_PED_ALERTNESS(planePilot, 1000);
								PED::SET_PED_COMBAT_RANGE(planePilot, 1000);*/
								ENTITY::SET_ENTITY_INVINCIBLE(planePilot, 1);
								Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(planePilot, 0);
								//WAIT(0);
								if (TASK::GET_SCRIPT_TASK_STATUS(planePilot, 0x21d33957) == 7)
								{
									TASK::TASK_VEHICLE_DRIVE_TO_COORD(planePilot, vehicle, wpVec.x, wpVec.y, wpVec.z + rand() % 21, (float)(rand() % 33 + 69), 1, ENTITY::GET_ENTITY_MODEL(vehicle), 16777216 || 262144, -1.0, -1.0);
								}
								timerPlaned = MISC::GET_GAME_TIMER() + delayedPlanned;
							}
						}

					}
					/*Ped playerPed = PLAYER::PLAYER_PED_ID();
					Entity detentity = PLAYER::PLAYER_PED_ID();*/
					Ped playerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player);
					
					if (PED::IS_PED_IN_ANY_VEHICLE(detentity, 0))
						detentity = PED::GET_VEHICLE_PED_IS_USING(detentity);
					if (neverWanted)
					{
						PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID());
						PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true);
						PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID(), 0, 1);
					}
					if (airstrikegun)
					{
						Vector3 iCoord;
						if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
						{
							MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(iCoord.x, iCoord.y + 5, iCoord.z + 5, iCoord.x, iCoord.y, iCoord.z, 100, 0, MISC::GET_HASH_KEY("WEAPON_RPG"), PLAYER::PLAYER_PED_ID(), 0, 0, 50);

						}
						
					}
					
					/*int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), vehs);*/
					int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), vehs);
					//int count = worldGetAllVehicles(ElementAmount, *vehs);
					if (vehs != NULL)
					{
						for (int i = 0; i < count; i++)
						{
							int offsettedID = i * 2 + 2;
							/*if (vehs[offsettedID] != PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false))*/
							if (vehs[offsettedID] != PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false))
							{
								if (vehs[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
								{
									if (VEHICLE::IS_VEHICLE_MODEL(vehs[offsettedID], PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)]))
									{
										Vector3 planeCoords = ENTITY::GET_ENTITY_COORDS(vehs[offsettedID], 0);
										if (MISC::GET_DISTANCE_BETWEEN_COORDS(planeCoords.x, planeCoords.y, planeCoords.z, coords.x, coords.y, coords.z, 1) < 69.99999f)
										{
											float deuxcenthuitcent = 200 % 800;
											
												/*GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(planeCoords.x, planeCoords.y + 5, planeCoords.z + GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(6.337, 21.337), coords.x, coords.y, coords.z, 100, 0, GAMEPLAY2::GET_HASH_KEY2("WEAPON_RPG"), PLAYER::PLAYER_PED_ID(), 0, 0, rand() + 200 % 800);*/
												MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(planeCoords.x, planeCoords.y + 5, planeCoords.z + MISC::GET_RANDOM_FLOAT_IN_RANGE(6.337, 21.337), coords.x, coords.y, coords.z, 100, 0, MISC::GET_HASH_KEY("WEAPON_RPG"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 0, 0, rand() + deuxcenthuitcent);
											
											
										}
									}
								}
							}
						}
					}
				
			
		}
		else
		{
			ENTITY::DELETE_ENTITY(vehs);
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
		if (noclip)
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
		
		else
		{
			PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), false);
		}
		if (modifytimecycle)
		{
			MISC::SET_TIME_SCALE(features::timescale);
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

					if (godmode)
					{
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), godmode);

					}
					else
					{
						ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
					}

					if (offradar) {

						//*script_global(2689224).at(PLAYER::GET_PLAYER_INDEX(), 421).at(207).as<int*>() = offradar;
						//*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;
						*script_global(2689224).at(PLAYER::GET_PLAYER_INDEX(), 451).at(207).as<int*>() = offradar;
						*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;

					}
					else {
						//*script_global(2439138).at(70).as<int*>() = 0;
					}
					if (superrunbool)
					{
						notify("Hold SHIFT To Run", "Controls", 3000);
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
					else
					{
						Entity entity = (PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));
						ENTITY::SET_ENTITY_MAX_SPEED(entity, 540);
					}
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
							GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
							STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
							GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_trev1_trailer_boosh");
							GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false);
						}
						else
						{
							MISC::SET_TIME_SCALE(1);
							GRAPHICS::SET_TIMECYCLE_MODIFIER("li");;
						}
					}
					else
					{
						//GRAPHICS::SET_TIMECYCLE_MODIFIER("li");
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
					if (trafficfollowplayer)
					{
						Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1);
						float heading = (ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0)).z;
						FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
						if (MISC::GET_GAME_TIMER() > timerPlaned)
						{




							int vehicles = VEHICLE::CREATE_VEHICLE(VehsToFollow[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*/5)], coords.x + rand() % 339, coords.y + rand() % 669, coords.z + rand() % 29 + 69, lookDir, 1, 1, 1);
							//int SpawnedPlanes = CHooking::create_vehicle(PlanesToBeAngry[MISC::GET_RANDOM_INT_IN_RANGE(1, /*18*//*25*/50)], &Coordsx, lookDir, 1, 1);
							Ped planePilot = PED::CREATE_RANDOM_PED_AS_DRIVER(vehicles, false);
							PED::SET_DRIVER_ABILITY(planePilot, 1000.0f);
							PED::SET_DRIVER_AGGRESSIVENESS(planePilot, 1000.0f);
							/*PED::SET_PED_COMBAT_ABILITY(planePilot, 100);
							AI::TASK_COMBAT_PED(planePilot, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), 1, 1);
							PED::SET_PED_ALERTNESS(planePilot, 1000);
							PED::SET_PED_COMBAT_RANGE(planePilot, 1000);*/
							ENTITY::SET_ENTITY_INVINCIBLE(planePilot, 1);
							Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(planePilot, 0);
							//WAIT(0);
							if (TASK::GET_SCRIPT_TASK_STATUS(planePilot, 0x21d33957) == 7)
							{
								TASK::TASK_VEHICLE_DRIVE_TO_COORD(planePilot, vehicle, coords.x, coords.y, coords.z + rand() % 21, (float)(rand() % 33 + 69), 1, ENTITY::GET_ENTITY_MODEL(vehicle), 16777216 || 262144, -1.0, -1.0);
							}
							timerPlaned = MISC::GET_GAME_TIMER() + delayedPlanned;


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
					break;
				case 1:
					////////////////////////////////////////   250ms   ////////////////////////////////////////
					if (unlimitedstamina)
					{
						PLAYER::GET_PLAYER_SPRINT_STAMINA_REMAINING(PLAYER::PLAYER_PED_ID());
						PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_PED_ID(), 100);
					}
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
						*script_global(2426865).at((PLAYER::GET_PLAYER_INDEX(), 451)).at(207).as<int*>() = 1;
						
						*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;
						

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
					if (selfdrop)
					{
						Ped iPed = PLAYER::PLAYER_PED_ID();
						Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(iPed, 0);
						OBJECT::CREATE_AMBIENT_PICKUP(0x1E9A99F8, pCoords.x, pCoords.y, pCoords.z, 0, 2500, -1666779307, FALSE, TRUE);// This works and all but seems to cause native execution errors on slower PC's.  
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(-1666779307);
					}

					/*if (dropnotify)
					{
						
						inline const char* drophashes[] = {
							"0xCE6FDD6B", "-1666779307", "0x1E9A99F8"
						};


						Vector3 g_Pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);

						if (OBJECT::_IS_PICKUP_WITHIN_RADIUS(&drophashes, g_Pos.x, g_Pos.y, g_Pos.z, 9999.0f)) {

							notifyMap("~r~ALERT~s~: Cash Is Being Dropped In This Session!");

						}

						if (OBJECT::_IS_PICKUP_WITHIN_RADIUS(drophashes, g_Pos.x, g_Pos.y, g_Pos.z, 5.0f)) {
							notifyMap("~r~ALERT~s~: Someone's Attempting To Drop Cash On You!");
						}
					}*/
					break;
				case 3:
					//25003ms
					
					
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
