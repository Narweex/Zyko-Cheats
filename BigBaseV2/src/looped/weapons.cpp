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
	void features::weapons_loop()
	{
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
		if (infiniteammo)
		{
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), true);
		}
		else
		{
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), false);
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
	}
	void features::RemoveAllWeapons() { WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), true); }
	void features::MaxAmmo() {
		Hash Ammo;	WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &Ammo, 1);
		WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), Ammo, 9999, 9999);
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
}