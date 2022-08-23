#include "../common.hpp"
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
	void features::player_loop()
	{
		if (neverWanted)
		{
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);
		}
		else
		{
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
		}

		if (godmode)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), godmode);
		}
		if (ignoreplayer)
		{
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true);
		}
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
		if (offradar) {

			/**script_global(2689224).at(PLAYER::GET_PLAYER_INDEX(), 451).at(207).as<int*>() = 1;
			*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;*/
			*script_global(2689235).at(PLAYER::GET_PLAYER_INDEX(), 453).at(208).as<int*>() = 1;
			*script_global(2703735).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;
		}
		if (modifytimecycle)
		{
			MISC::SET_TIME_SCALE(features::timescale);
		}
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
		if (forcefield) {
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), forcefield);
			Player playerPed = PLAYER::PLAYER_PED_ID();
			PED::SET_PED_CAN_RAGDOLL(playerPed, false);
			Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
			FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 7, 5.00f, 0, 1, 0, 1);

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
				}}}
		if (unlimitedstamina)
		{
			PLAYER::GET_PLAYER_SPRINT_STAMINA_REMAINING(PLAYER::PLAYER_PED_ID());
			PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_PED_ID(), 100);
		}
		if (noidlekick) { features::noIdleKick(); }
		if (offradar){
			*script_global(2426865).at((PLAYER::PLAYER_PED_ID(), 451)).at(207).as<int*>() = 1;
			*script_global(2703660).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;}

		if (bullshark){*script_global(2703660).at(3576).as<int*>() = 1;}
		if (features::nightvision) {	GRAPHICS::SET_NIGHTVISION(true);}
		else {GRAPHICS::SET_NIGHTVISION(false);}

		if (features::thermalvision) {GRAPHICS::SET_SEETHROUGH(true);}
		else {GRAPHICS::SET_SEETHROUGH(false);}

	}





	/* Voids */
	void features::maxhealth() { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 400, 100); }
	void features::maxarmor() { PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200); }
	void features::setwanted() {
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
	}
	void features::noIdleKick(){
		*script_global(Tunables).at(87).as<int*>() = INT_MAX;
		*script_global(Tunables).at(88).as<int*>() = INT_MAX;
		*script_global(Tunables).at(89).as<int*>() = INT_MAX;
		*script_global(Tunables).at(90).as<int*>() = INT_MAX;
		*script_global(Tunables).at(7785).as<int*>() = INT_MAX;
		*script_global(Tunables).at(7786).as<int*>() = INT_MAX;
		*script_global(Tunables).at(7787).as<int*>() = INT_MAX;
		*script_global(Tunables).at(7788).as<int*>() = INT_MAX;
		*script_global(IdleTimer).at(IdleTimer_Offset_1).as<int*>() = 0;
		*script_global(IdleTimer).at(IdleTimer_Offset_2).as<int*>() = 0;
		*script_global(IdleTimerUNK1).as<int*>() = 0;
		*script_global(IdleTimerUNK2).as<int*>() = 0;
	}

}