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

namespace big
{
	void features::on_present()
	{
		TRY_CLAUSE
		{
		}
		EXCEPT_CLAUSE
	}


	///////////////////////////////////////////////////////   HELP VOIDS   ///////////////////////////////////////////////////////
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
					if (neverWanted)
					{
						PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID());
						PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true);
					}
					else
					{
						PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), false);
					}
					if (features::offradar) {
						*script_global(2689224).at(PLAYER::PLAYER_ID(), 421).at(207).as<int*>() = 1;
						*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 75497245;
						
					}
					else {
						if (*g_pointers->m_is_session_started)
							*script_global(2439138).at(70).as<int*>() = 0;
					}
					if (superrunbool)
					{
						notifyMap("Hold SHIFT To Activate");
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
						
						Player playerPed = PLAYER::PLAYER_PED_ID();
						PED::SET_PED_CAN_RAGDOLL(playerPed, false); 
						Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 16, 5.00f, 0, 1, 0, 0);

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
					if (features::exploammo)
					{	
						Vector3 iCoord;
						if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &iCoord))
						{
							FIRE::ADD_EXPLOSION(iCoord.x, iCoord.y, iCoord.z, 25, 10000.0f, true, false, 0, false);
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
						}
					}
					else
					{
						//GRAPHICS::SET_TIMECYCLE_MODIFIER("li");;
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
											if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, targetPed, 17)&& PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
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
