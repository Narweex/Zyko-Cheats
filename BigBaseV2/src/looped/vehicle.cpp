#include "../common.hpp"
#include "features.hpp"
namespace zyko
{
	
	

	bool features::instantenter = false;
	void features::Instantenter(bool toggle)
	{
		if (!toggle)
			return;
		if (PED::IS_PED_GETTING_INTO_A_VEHICLE(PLAYER::PLAYER_PED_ID()))
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
	}

	bool features::vehgodmode = false;
	void features::Vehgodmode(bool toggle)
	{
		if (toggle)
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

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
	}

	bool features::speedbypass = false;
	void features::Speedbypass(bool toggle)
	{
		Entity entity = (PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));
		ENTITY::SET_ENTITY_MAX_SPEED(entity, 5000);
		ENTITY::SET_ENTITY_MAX_SPEED(PLAYER::PLAYER_PED_ID(), 5000);
	}
	
	bool features::hornboost = false;
	void features::Hornboost(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			return;
		if (toggle)
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
	}

	bool features::smoothhornboost = false;
	void features::Smoothhornbbost(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			return;
		if (toggle)
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
	}

	bool features::seatbelt = false;
	void features::Seatbelt(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			return;
		if (toggle)
		{
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 1);
		}
	}

	bool features::novehkick = false;
	void features::Novehkick(bool toggle)
	{
		if (features::novehkick)
		{
			*script_global(2689224).at(PLAYER::PLAYER_PED_ID()).at(317).at(10).as<int*>() = 21501;
			*script_global(1958845).as<int*>() = 1;
			script_global(262145).at(7478);


		}
	}

	bool features::fixloop = false;
	void features::Fixloop(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			return;
		if (toggle)
		{
			VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
		}
	}

	bool features::cleanloop = false;
	void features::Cleanloop(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			return;
		if (cleanloop)
		{
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())), 0);
		}
	}

	bool features::rainbowcar = false;
	void features::Rainbowcar(bool toggle)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			return;
		if (toggle)
		{	
			features::rainbowloop();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, features::VEHr, features::VEHg, features::VEHb);
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, features::r1, features::g1, features::b1);
			
		}
	}

	bool features::flashrainbow = false;
	void features::Flashrainbow(bool toggle)
	{
		if (toggle)
		{
			features::colour_spam();
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
			{
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, r1, g1, b1);
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, r1, g1, b1);
			}
		}
	}

	bool features::invis_car = false;
	void features::Invis_car(bool toggle)
	{
		if (toggle)
		{
			const Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			ENTITY::SET_ENTITY_ALPHA(vehicle, 0, true);
		}
		else
		{
			ENTITY::SET_ENTITY_ALPHA(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), 255, true);

		}
	}

	void features::AddRamp()
	{
		Vector3 Him = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		Entity Ramp = OBJECT::CREATE_OBJECT(0xB157C9E4, Him.x, Him.y, Him.z, 1, 1, 0);
		int Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), true);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			STREAMING::REQUEST_MODEL(0xB157C9E4);
			if (STREAMING::HAS_MODEL_LOADED(0xB157C9E4))
			{
				if (ENTITY::DOES_ENTITY_EXIST(Ramp))
				{
					ENTITY::SET_ENTITY_ALPHA(Ramp, 200, false);
					ENTITY::SET_ENTITY_COLLISION(Ramp, true, true);
					ENTITY::ATTACH_ENTITY_TO_ENTITY(Ramp, Vehicle, 0, 0, 6.3, -0.5, 13.5, 0, 177.5, 0, 0, true, 0, 2, 1);
				}
			}
		}
	}




	void features::rainbowloop()
	{
		if (features::VEHr > 0 && features::VEHb == 0)
		{
			features::VEHr--;
			features::VEHg++;
		}
		if (features::VEHg > 0 && features::VEHr == 0)
		{
			features::VEHg--;
			features::VEHb++;
		}
		if (features::VEHb > 0 && features::VEHg == 0)
		{
			features::VEHr++;
			features::VEHb--;
		}
	}

	void features::colour_spam()
	{
		r1 = MISC::GET_RANDOM_INT_IN_RANGE(0, 250);
		g1 = MISC::GET_RANDOM_INT_IN_RANGE(0, 250);
		b1 = MISC::GET_RANDOM_INT_IN_RANGE(0, 250);

	}

	void features::maxvehicle(const int& VehicleHandle)
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
	void features::spawn_veh(const Hash& vehicle)
	{
		
		if (!STREAMING::HAS_MODEL_LOADED(vehicle))
		{
			STREAMING::REQUEST_MODEL(vehicle);
			script::get_current()->yield();
		}

		auto pos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x9090;
		auto veh = VEHICLE::CREATE_VEHICLE(vehicle, pos.x + 5, pos.y + 5, pos.z, 0.f, TRUE, FALSE, FALSE);
		*(unsigned short*)g_pointers->m_model_spawn_bypass = 0x0574;



		script::get_current()->yield();
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicle);
		if (*g_pointers->m_is_session_started)
		{
			DECORATOR::DECOR_SET_INT(veh, xorstr_("MPBitset"), 0);
			ENTITY::_SET_ENTITY_SOMETHING(veh, TRUE);
			auto networkId = NETWORK::VEH_TO_NET(veh);
			if (NETWORK::NETWORK_GET_ENTITY_IS_NETWORKED(veh))
				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(networkId, true);
			VEHICLE::SET_VEHICLE_IS_STOLEN(veh, FALSE);
			//VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "Zyko");
		}

		if (features::in_vehicle) {
			if (VEHICLE::IS_THIS_MODEL_A_HELI(vehicle) || VEHICLE::IS_THIS_MODEL_A_PLANE(vehicle))
				VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, true);
			VEHICLE::SET_HELI_BLADES_FULL_SPEED(veh);

			if (VEHICLE::IS_THIS_MODEL_A_PLANE(vehicle))
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
	void features::cleanVehicle()
	{
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())), 0);
	}
	void features::repairVehicle()
	{
		VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
	}
	void features::acrobatics(int i)
	{
		const int veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
		if (i == 1) {
			// forward flip
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, true, 0, 0, 17.f, 0, -1.f, 2.f, true, true, true, true, false, true);
		}
		else if (i == 2) {
			// Back flip
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, true, 0, 0, 17.f, 0, 1.f, 2.f, true, true, true, true, false, true);
		}
		else if (i == 3) {
			// left flip
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, true, 0, 0, 10.f, 10.f, 0, 0, true, true, true, true, false, true);
		}
		else if (i == 4) {
			// right flip
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, true, 0, 0, 10.f, -10.f, 0, 0, true, true, true, true, false, true);
		}
		else if (i == 5) {
			// jump up
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, true, 0, 2, 10.0f, 0, 0, 0, true, true, true, true, false, true);
		}
		else if (i == 6) {
			// launch up
			ENTITY::APPLY_FORCE_TO_ENTITY(veh, true, 0, 0, 1000.0f, 0, 0, 0, true, true, true, true, false, true);
		}
	}

	void features::SetNumberplate(const char* text)
	{
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), text);
	}

	void features::perform()
	{
		ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), true, features::x, features::y, features::z, features::offX, features::offY, features::offZ, true, true, true, true, false, true);
	}
}