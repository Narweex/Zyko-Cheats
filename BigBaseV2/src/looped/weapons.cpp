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
}