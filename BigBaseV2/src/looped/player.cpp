#include "../common.hpp"
#include "features.hpp"
namespace zyko
{

	bool features::godmode = false;
	void features::Godmode(bool toggle)
	{
		if (toggle)
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
		}
		else
		{
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);

		}

	}

	bool features::neverwanted = false;
	void features::Neverwanted(bool toggle)
	{
		if (neverwanted)
		{
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			//PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, false);
		}
		else
		{
			PLAYER::SET_MAX_WANTED_LEVEL(5);
		}


	}




	bool features::superman = false;
	void features::Superman(bool toggle)
	{
		
		if (toggle && gta_util::IsKeyPressed(VK_SPACE))
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
		else
		{
			NULL;
		}
	}

	bool features::noclip = false;
	void features::Noclip(bool toggle)
	{
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
			ENTITY::FREEZE_ENTITY_POSITION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true), false);
			ENTITY::SET_ENTITY_COLLISION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true), true, false);
		}
	}


	bool features::flashrun = false;
	void features::Flashrun(bool toggle)
	{
		if (toggle)
		{
			if (gta_util::IsKeyPressed(VK_SHIFT))
			{
				PED::SET_PED_MOVE_RATE_OVERRIDE(PLAYER::PLAYER_PED_ID(), 3.0);
				GRAPHICS::SET_TIMECYCLE_MODIFIER("RaceTurboFlash");
				MISC::SET_TIME_SCALE(0.7);
				GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_trev1_trailer_boosh");
				GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1, true, true, true);
				GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1, true, true, true);

			}
			else
			{
				MISC::SET_TIME_SCALE(1);
				GRAPHICS::SET_TIMECYCLE_MODIFIER("li");;
			}
		}
		else
		{
			MISC::SET_TIME_SCALE(1);
			//GRAPHICS::SET_TIMECYCLE_MODIFIER("li");;
		}
	}

	bool features::invisibility = false;
	void features::Invisibility(bool toggle)
	{
		if (toggle)
		{
			ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 0, false);
		}
		else
		{
			ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), features::playeralpha, false);
		}
	}

	bool features::norag = false;
	void features::Norag(bool toggle)
	{
		if (toggle)
		{
			PED::SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), false);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), false);
			PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), false);
		}
		else
		{
			PED::SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), true);
		}

	}

	bool features::semigod = false;
	void features::Semigod(bool toggle)
	{
		if (features::semigod)
		{
			PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_PED_ID(), 1000.f);
		}
	}

	bool features::bullshark = false;
	void features::Bullshark(bool toggle)
	{
		if (toggle)
		{
			*script_global(2703660).at(3576).as<int*>() = 1;
		}
	}

	bool features::offradar = false;
	void features::Offradar(bool toggle)
	{
		if (toggle) {
			*script_global(2689235).at(PLAYER::GET_PLAYER_INDEX(), 453).at(208).as<int*>() = 1;
			*script_global(2703735).at(56).as<int*>() = NETWORK::GET_NETWORK_TIME() + 1;
		}
	}

	bool features::mobileradio = false;
	void features::Mobileradio(bool toggle)
	{
		if (toggle)
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else
		{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}

	bool features::hide_hud = false;
	void features::Hide_hud(bool toggle)
	{
		if (toggle)
		{
			HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		}
	}

	bool features::superjumpbool = false;
	void features::Superjump(bool toggle)
	{
		if (toggle)
		{
			MISC::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
		}
	}

	bool features::ultrajumpbool = false;
	void features::Ultrajumpbool(bool toggle)
	{
		if (toggle)
		{
			if (PED::IS_PED_JUMPING(PLAYER::PLAYER_PED_ID()))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), true, 0, 0, 150, 0, 0, true, true, true, true, false, true, false);
			}
		}
	}

	bool features::superrunbool = false;
	void features::Superrun(bool toggle)
	{
		if (toggle)
		{
			if (gta_util::IsKeyPressed(VK_SHIFT))
			{
				PED::SET_PED_MOVE_RATE_OVERRIDE(PLAYER::PLAYER_PED_ID(), runspeed);
			}
		}
	}

	bool features::forcefield = false;
	void features::Forcefield(bool toggle)
	{
		if (toggle) {
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
			Player playerPed = PLAYER::PLAYER_PED_ID();
			PED::SET_PED_CAN_RAGDOLL(playerPed, false);
			Vector3 pCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
			FIRE::ADD_EXPLOSION(pCoords.x, pCoords.y, pCoords.z, 7, features::forcefield_force, 0, 1, 0, 1);

		}
	}
	
	bool features::ignoreplayer = false;
	void features::Ignoreplayer(bool toggle)
	{
		if (toggle)
		{
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_PED_ID(), true);
		}
	}

	bool features::ultrarunbool = false;
	void features::Ultrarunbool(bool toggle)
	{
			if (toggle && gta_util::IsKeyPressed(VK_SHIFT))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0.f, 1.5f, 0.f, 0.f, 0.f, 0.f, 1, true, true, true, false, true);
			}
	}

	bool features::rotating = false;
	void features::Rotating(bool toggle)
	{
		if (toggle && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			float heading = ENTITY::_GET_ENTITY_PHYSICS_HEADING(PLAYER::PLAYER_PED_ID());
			ENTITY::SET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID(), heading + 5);
		}
	}





	bool features::unlimitedstamina = false;
	void features::Unlimitedstamina(bool toggle)
	{
		if (toggle)
		{
			PLAYER::GET_PLAYER_SPRINT_STAMINA_REMAINING(PLAYER::PLAYER_PED_ID());
			PLAYER::RESTORE_PLAYER_STAMINA(PLAYER::PLAYER_PED_ID(), 100);
		}
	}
		
		
	
	bool features::nightvision = false;
	void features::Nightvision(bool toggle)
	{
		if(!toggle)
			GRAPHICS::SET_NIGHTVISION(false);

		GRAPHICS::SET_NIGHTVISION(toggle);
		
		
		
	}
		
	bool features::thermalvision = false;
	void features::Thermalvision(bool toggle)
	{
		if(!toggle)
			GRAPHICS::SET_SEETHROUGH(toggle);
		GRAPHICS::SET_SEETHROUGH(toggle);

	}
		





	/* Voids */
	void features::maxhealth() { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 400, 100); }
	void features::maxarmor() { PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200); }
	void features::setwanted() {
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
	}

	bool features::noidlekick = false;
	void features::Noidlekick(bool toggle)
	{
		if (toggle)
		{
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

	void features::PlayAnimation(const char* name, const char* id)
	{
		features::RequestControlOfEnt(PLAYER::PLAYER_PED_ID());
		ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), true);
		STREAMING::REQUEST_ANIM_DICT(name);
		if (STREAMING::HAS_ANIM_DICT_LOADED((name)))
		{
			TASK::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), name, id, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
		}
		
	}

	void features::StopAnimation()
	{
		TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
	}

	void features::Set_wetness(const float& level)
	{
		PED::SET_PED_WETNESS_HEIGHT(PLAYER::PLAYER_PED_ID(), features::set_wetness);
	}

}