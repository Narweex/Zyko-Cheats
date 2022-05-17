#include "common.hpp"
#include "features.hpp"
#include "logger.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "gta_util.hpp"
#include "helpers/player_info.h"
#include "script_global.hpp"
#include "gta_util.hpp"
#include "../../BigBaseV2/math.hpp"
#include <control.h>
#include <script_local.hpp>
#include <sstream>


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
	///////////////////////////////////////////////////////   SPAWN VOIDS   ///////////////////////////////////////////////////////
	void features::spawn_obj(const char* object)
	{
		const char* obj = (const char*)object;
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
		OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY(obj), pos.x, pos.y, pos.z, 1, 1, 1);
	}
	void features::play_particle(const char* particle)
	{
		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL(particle);
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
				GRAPHICS::USE_PARTICLE_FX_ASSET(xorstr_("scr_rcbarry2"));
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
	void notifyMap(std::string content)
	{
		NULL;
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
		
		
		
		
		

		for (int i = 0; i < sizeof(tick_conf) / sizeof(ULONGLONG); i++)
		{
			if (now - ticks[i] > tick_conf[i])
			{
				g_current_tick_job = i;

				switch (g_current_tick_job)
				{
				case 0:
					//50 MS
				
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
						PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID(), 0, true);
		
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
					if (forcefield) {
						
						Player playerPed = PLAYER::PLAYER_PED_ID();
						PED::SET_PED_CAN_RAGDOLL(playerPed, false);
						Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z+0.1f, 7, 9.0f, false, true, 0.0f, 1);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z+0.1f, 7, 9.0f, false, true, 0.0f, 1);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z+0.1f, 7, 9.0f, false, true, 0.0f, 1);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z+0.1f, 7, 9.0f, false, true, 0.0f, 1);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z+0.1f, 7, 9.0f, false, true, 0.0f, 1);
						FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z+0.1f, 7, 9.0f, false, true, 0.0f, 1);

					}
					if (freecam)
					{
						Ped playerPed = PLAYER::PLAYER_PED_ID();
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
						if (gta_util::IsKeyPressed(0x53) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 268)) {
							float fivef = 5.5f;
							float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
							float xVec = fivef * sin(degToRad(heading)) * -1.0f;
							float yVec = fivef * cos(degToRad(heading));
							ENTITY::SET_ENTITY_HEADING(playerPed, heading);

							pos.x -= xVec, pos.y -= yVec;
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
						}
						if (gta_util::IsKeyPressed(0x57) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 269)) {
							float fivef = .5f;
							float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
							float xVec = fivef * sin(degToRad(heading)) * -1.0f;
							float yVec = fivef * cos(degToRad(heading));
							ENTITY::SET_ENTITY_HEADING(playerPed, heading);

							pos.x += xVec, pos.y += yVec;
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
						}
						if (gta_util::IsKeyPressed(0x41) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 266)) {
							float fivef = .5f;
							float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
							ENTITY::SET_ENTITY_HEADING(playerPed, heading + 0.5f);
						}
						if (gta_util::IsKeyPressed(0x44) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 271)) {
							float fivef = .5f;
							float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
							ENTITY::SET_ENTITY_HEADING(playerPed, heading - 0.5f);
						}
						if (gta_util::IsKeyPressed(VK_SHIFT)) {
							float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
							ENTITY::SET_ENTITY_HEADING(playerPed, heading);

							pos.z -= 0.5;
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
						}
						if (gta_util::IsKeyPressed(VK_SPACE)) {
							float heading = ENTITY::GET_ENTITY_HEADING(playerPed);
							ENTITY::SET_ENTITY_HEADING(playerPed, heading);

							pos.z += 0.5;
							ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, pos.x, pos.y, pos.z, false, false, false);
						}
					}
					if (deletegun)
					{
						Entity EntityTarget;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &EntityTarget) && gta_util::IsKeyPressed(VK_LBUTTON))
						{
							Vector3 EntityTargetPos = ENTITY::GET_ENTITY_COORDS(EntityTarget, 0);
							PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_ID(), true);
							RequestControlOfEnt(EntityTarget);
							EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(EntityTarget, true, true);
							VEHICLE::DELETE_VEHICLE(&EntityTarget);
							ENTITY::DELETE_ENTITY(&EntityTarget);
						}
						PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_ID(), false);
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
					if (playeresp)
					{

						Player playerPed = PLAYER::PLAYER_PED_ID();
						for (int i = 0; i < 32; i++)
						{
							Player playerHandle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i);
							Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
							char* Name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));

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
											if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, targetPed, 17))
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

					if (ultrajumpbool)
					{
						MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_PED_ID());
						MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
						if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
						{
							ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), true, 0, 0, 150, 0, 0, true, true, true, true, false, true, false);
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
					break;
				case 3:
					//25003ms
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
