#include "MainScript.hpp"
#include "../Lists.hpp"
#include "Natives.hpp"
#include "Timer.hpp"
#include "../UIManager.hpp"
#include "../BoolOption.hpp"
#include "../ChooseOption.hpp"
#include "../NumberOption.hpp"
#include "../RegularOption.hpp"
#include "../SubOption.hpp"
#include "../RegularSubmenu.hpp"
#include "../PlayerSubmenu.hpp"
#include "features.hpp"
#include "gui.hpp"
#include "gta/joaat.hpp"
#include "script_global.hpp"

namespace big
{
	enum Submenu : std::uint32_t
	{
		SubmenuHome,
		SubmenuSettings,
		SubmenuSettingsHeader,
		SubmenuSettingsHeaderStaticBackground,
		SubmenuSettingsHeaderGradientBackground,
		SubmenuSettingsHeaderText,
		SubmenuSettingsSubmenuBar,
		SubmenuSettingsOption,
		SubmenuSettingsFooter,
		SubmenuSettingsDescription,
		SubmenuSettingsInput,
		SubmenuSettingsLanguage,
		SubmenuSelectedPlayer,
		SubmenuPlayerList,
		SubmenuTest,
		SubmenuSelf,
		SubmenuVehicleMultipliers,
		SubmenuNetwork,
		SubmenuSpawner,
		vis,
		SubmenuVehicle,
		SubmenuVehicleSpawner,
		SubmenuWeapons,
		SubmenuWorld,
		SubmenuTeleport,
		SubmenuMiscellaneous,
		Weaponz,
		money,
		stats,
		unlocks,
		SubmenuVehicleColors,
		OnlineSubmenu,
		VehicleOptions,
		teleports,
		WorldOptions,
		SelectedPlayerAbuse,
		SelectedPlayerFriendly,
		SelectedPlayerTeleport,
		SelectedPlayerTrolling,
		recovery,
		SubmenuVehicleMovement,
		misc,
		Protections,
		WaterSubmenu,
		TimeSubmenu,
		WeatherSubmenu,
		landmarks,
		shops,
		air,
		ipls,
		gar,
		safehouse,
		ClearSubmenu,
		SessionInfoSubmenu
	};

	void MainScript::gui_init()
	{
	}

	void MainScript::gui_on_tick()
	{
		g_UiManager->OnTick();
	}
	void teleport(float x, float y, float z)
	{
		PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), x, y, z);
	}

	void MainScript::script_init()
	{
		m_Initialized = true;
		using namespace UserInterface;

		g_UiManager->AddSubmenu<RegularSubmenu>("Home", SubmenuHome, [](RegularSubmenu* sub)
		{
				sub->AddOption<SubOption>("Self", "Self Options", SubmenuSelf);
				sub->AddOption<SubOption>("Vehicle Options", "Vehicle Options", SubmenuVehicle);
				sub->AddOption<SubOption>("Weapons", "Weapon Options", Weaponz);
				sub->AddOption<SubOption>("Online", "Online Options", OnlineSubmenu);
				sub->AddOption<SubOption>("Teleport", "Teleport Options", teleports);
				sub->AddOption<SubOption>("World Options", "World Options", WorldOptions);
				sub->AddOption<SubOption>("Recovery", "Recovery Options", recovery);
				sub->AddOption<SubOption>("Misc Options", "Other Options", misc);
				sub->AddOption<SubOption>("Protections", "Protection Options", Protections);
				sub->AddOption<SubOption>("Settings", "Menu settings", SubmenuSettings);
			
		});

		g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Demo_sub"), SubmenuSelf, [](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Visions", "Change Player Visions", vis);
				sub->AddOption<BoolOption<bool>>("God Mode", "You Cannot Die", &features::godmode, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Never Wanted", "Police Wont Start Coming AFter You", &features::neverWanted, BoolDisplay::OnOff);
				if (!features::neverWanted) {
					sub->AddOption<NumberOption<int>>("Wanted Level", nullptr, &features::wantedLevel, 0, 5, 1, 3, true, "", "", []
						{
							PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
							PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
						});
				}

				sub->AddOption<BoolOption<bool>>("Super Run - Override", "Run Boiiiiiii", &features::superrunbool, BoolDisplay::OnOff);
				if (features::superrunbool) {
					sub->AddOption<NumberOption<float>>("Run Speed", nullptr, &features::runspeed, 0.1, 5.0, 0.1, 3, true, "", "", [] {});
				}
				sub->AddOption<BoolOption<bool>>("Super Run - Force", "Run Boiiiiiii", &features::ultrarunbool, BoolDisplay::OnOff);
				if (features::ultrarunbool) {
					sub->AddOption<NumberOption<float>>("Run Speed", nullptr, &features::runspeed1, 0.1, 5.0, 0.1, 3, true, "", "", [] {});
				}
				sub->AddOption<BoolOption<bool>>("Flash Mode", "Run Like Flash", &features::flashrun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Super Jump", "Ever wanted to jump higher?", &features::superjumpbool, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Ultra Jump", "Jump Really High", &features::ultrajumpbool, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Forcefield", "Push everyone", &features::forcefield, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clear Wanted", "Clear Player Wanted Level", []	
				{
					PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
				});
					
				sub->AddOption<RegularOption>("Max Health", "Adds Health To The Player", []
					{
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 400, 100);
					});
				sub->AddOption<RegularOption>("Max Armour", "Adds Armour To The Player", []
					{
						PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200);
					});
				sub->AddOption<BoolOption<bool>>("InfiniteStamina", "You Can Just Run Forever", &features::unlimitedstamina, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clean Ped", "Clean Player", []
					{
						PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
					});
				sub->AddOption<RegularOption>("Randomise Clothes", "Randomises Players Clothes", []
					{
						PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), true);
					});
				sub->AddOption<RegularOption>("Reset Ped", "Reset Player", []
					{
						PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
						PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
						PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
					});
				sub->AddOption<RegularOption>("Suicide", "Kills Ped", []
					{
						ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, 0);
					});
			sub->AddOption<RegularOption>(xorstr_("RegularOption_demo"), "A regular option.", []
			{
				LOG(INFO) << "Test Option pressed";
			});
			g_UiManager->AddSubmenu<RegularSubmenu>("Visions", vis, [](RegularSubmenu* sub)
				{
					sub->AddOption<RegularOption>("Clear", "Clears Vision", []
						{
							GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
						});
					sub->AddOption<RegularOption>("Bank HLWD", "Bank HLWD Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("Bank_HLWD");
						});
					sub->AddOption<RegularOption>("Barry Stoned", "Barry Stoned Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("Barry1_Stoned");
						});
					sub->AddOption<RegularOption>("Barry Fade Out", "Barry Fade Out Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("BarryFadeOut");
						});
					sub->AddOption<RegularOption>("Bloom Light", "Bloom Light Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("BloomLight");
						});
					sub->AddOption<RegularOption>("Building Top", "Building Top Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("buildingTOP");
						});
					sub->AddOption<RegularOption>("Bullet Time Dark", "Bullet Time Dark Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("BulletTimeDark");
						});
					sub->AddOption<RegularOption>("Building Time Light", "Building Time Light Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("BulletTimeLight");
						});
					sub->AddOption<RegularOption>("Camera ", "Camera Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("CAMERA_BW");
						});
					sub->AddOption<RegularOption>("Camera 2", "Camera 2 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("CAMERA_secuirity");
						});
					sub->AddOption<RegularOption>("Camera 3", "Camera 3 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("CAMERA_secuirity_FUZZ");
						});
					sub->AddOption<RegularOption>("Canyon Mission", "Canyon Mission Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("canyon_mission");
						});
					sub->AddOption<RegularOption>("Chop", "Chop Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("CHOP");
						});
					sub->AddOption<RegularOption>("Cinema", "Cinema Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("cinema");
						});
					sub->AddOption<RegularOption>("Cinema 2", "Cinema 2 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("cinema_001");
						});
					sub->AddOption<RegularOption>("Night Lighting", "Night Lighting Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("whitenightlighting");
						});
					sub->AddOption<RegularOption>("White Out", "White Out Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("WhiteOut");
						});
					sub->AddOption<RegularOption>("Tunnel", "Tunnel Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("Tunnel");
						});
					sub->AddOption<RegularOption>("Drugs 1", "Drugs 1 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("drug_drive_blend01");
						});
					sub->AddOption<RegularOption>("Drugs 2", "Drugs 2 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("drug_drive_blend02");
						});
					sub->AddOption<RegularOption>("Drugs 3", "Drugs 3 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("drug_flying_01");
						});
					sub->AddOption<RegularOption>("Race Turbo Dark", "Race Turbo Dark Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("RaceTurboDark");
						});
					sub->AddOption<RegularOption>("Race Turbo Flash", "Race Turbo Flash Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("RaceTurboFlash");
						});
					sub->AddOption<RegularOption>("Race Turbo Light", "Race Turbo Light Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("RaceTurboLight");
						});
					sub->AddOption<RegularOption>("Prologue Reflection", "prologue Reflection Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("prologue_reflection_opt");
						});
					sub->AddOption<RegularOption>("Prologue Shootout", "prologue Shootout Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("prologue_shootout");
						});
					sub->AddOption<RegularOption>("Prologue Shootout 2", "prologue Shootout 2 Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("Prologue_shootout_opt");
						});
					sub->AddOption<RegularOption>("Prologue", "prologue Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("prologue");
						});
					sub->AddOption<RegularOption>("Pulse", "pulse Vision", []
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER("pulse");
						});
				});

			
			sub->AddOption<BoolOption<bool>>(xorstr_("BoolOnOff_demo"), nullptr, &features::godmode, BoolDisplay::OnOff);
			static bool testBool2{};
			sub->AddOption<BoolOption<bool>>(xorstr_("BoolYesNo_demo"), nullptr, &testBool2, BoolDisplay::YesNo);

			static std::int32_t int32Test{ 69 };
			sub->AddOption<NumberOption<std::int32_t>>("Int32", nullptr, &int32Test, 0, 100);

			static std::int64_t int64Test{ 420 };
			sub->AddOption<NumberOption<std::int64_t>>("Int64", nullptr, &int64Test, 0, 1000, 10);

			static float floatTest{ 6.9f };
			sub->AddOption<NumberOption<float>>("Float", nullptr, &floatTest, 0.f, 10.f, 0.1f, 1);

			static std::vector<std::uint64_t> vector{ 1, 2, 3 };
			static std::size_t vectorPos{};

			sub->AddOption<ChooseOption<const char*, std::size_t>>("Array", nullptr, &Lists::DemoList, &Lists::DemoListPos);
			sub->AddOption<ChooseOption<std::uint64_t, std::size_t>>("Vector", nullptr, &vector, &vectorPos);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Movement", SubmenuVehicleMovement, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Vehicle Multipliers", nullptr, SubmenuVehicleMultipliers);
				if (features::smoothhornboost) {
					features::hornboost = false;
				}
				else
				{
					sub->AddOption<BoolOption<bool>>("Horn Boost", nullptr, &features::hornboost, BoolDisplay::OnOff);
				}
				sub->AddOption<BoolOption<bool>>("Smooth Horn Boost", nullptr, &features::smoothhornboost, BoolDisplay::OnOff);
				if (features::hornboost || features::smoothhornboost)
				{
					if (features::hornboost) {
						sub->AddOption<NumberOption<int>>("Horn Boost Speed", nullptr, &features::hornboostvalue, 0, 200, 10, 3, true, "", "", [] {});
					}
					sub->AddOption<BoolOption<bool>>("Horn Boost Effect", "Adds a Pog Turbo Effect", &features::hornboosteffect, BoolDisplay::OnOff);
				}
				sub->AddOption<NumberOption<std::int32_t>>("Vehicle Speed", nullptr, &features::vehiclespeed, 0, 500, 10, 3, true, "", "", [] {});
				sub->AddOption<RegularOption>("Set Speed", "", []
					{
						Vehicle Veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Veh);
						if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Veh))
						{
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(Veh, features::vehiclespeed);
						}
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Colors", SubmenuVehicleColors, [&](RegularSubmenu* sub)
			{
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~r~R~s~]", nullptr, &features::red, 0, 255, 10, 3, true, "", "", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~g~G~s~]", nullptr, &features::green, 0, 255, 10, 3, true, "", "", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~b~B~s~]", nullptr, &features::blue, 0, 255, 10, 3, true, "", "", [] {});
				sub->AddOption<RegularOption>("Set Vehicle Primary RGB", "Random Paint Vehicle", []
					{
						int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, features::red, features::green, features::blue);
					});
				sub->AddOption<NumberOption<std::int32_t>>("Color Secondary [~r~R~s~]", nullptr, &features::red2, 0, 255, 10, 3, true, "", "", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Secondary [~g~G~s~]", nullptr, &features::green2, 0, 255, 10, 3, true, "", "", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Secondary [~b~B~s~]", nullptr, &features::blue2, 0, 255, 10, 3, true, "", "", [] {});
				sub->AddOption<RegularOption>("Set Vehicle Secondary RGB", "Random Paint Vehicle", []
					{
						int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(VehID, features::red2, features::green2, features::blue2);
					});
				sub->AddOption<RegularOption>("Random Paint", "Random Paint Vehicle", []
					{
						int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
						VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(VehID, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle", SubmenuVehicle, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Vehicle Spawner", nullptr, SubmenuVehicleSpawner);
				sub->AddOption<SubOption>("Vehicle Movement", nullptr, SubmenuVehicleMovement);
				sub->AddOption<SubOption>("Vehicle Colors", nullptr, SubmenuVehicleColors);
				//sub->AddOption<BoolOption<bool>>("Vehicle Godmode", nullptr, &g_MainScript->vehiclegodmode, BoolDisplay::OnOff);

				sub->AddOption<RegularOption>("Repair Vehicle", "Repair Damages To Currnet Vehicle", []
					{
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
						VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
					});
				//sub->AddOption<RegularOption>("Set License Plate", "Set Custom Text", []
				//	{
				//		CharKeyboard("Text", 21, "nig");
				//		const char*  result = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				//		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false), result);
				//	});
				sub->AddOption<RegularOption>("Clean Vehicle", "Clean Currnet Vehicle", []
					{
						VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())), 0);
					});
				sub->AddOption<RegularOption>("Stop Vehicle", "Set Vehicle Stationary", []
					{
						Vehicle Veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Veh);
						if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Veh))
						{
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(Veh, 0);
						}
					});
			});
				g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle", SubmenuVehicleSpawner, [&](RegularSubmenu* sub)
			{
				
				sub->AddOption<SubOption>("Spawner Settings", nullptr, SubmenuVehSpawnerSettings);
				sub->AddOption<SubOption>("Sports", nullptr, SubmenuVehSpawnerSports);
				sub->AddOption<SubOption>("Muscle", nullptr, SubmenuVehSpawnerMuscle);
				sub->AddOption<SubOption>("Super", nullptr, SubmenuVehSpawnerSuper);
				sub->AddOption<SubOption>("Sport Classics", nullptr, SubmenuVehSpawnerSportClassics);
				sub->AddOption<SubOption>("Sedans", nullptr, SubmenuVehSpawnerSedans);
				sub->AddOption<SubOption>("SUVs", nullptr, SubmenuVehSpawnerSUVs);
				sub->AddOption<SubOption>("Compacts", nullptr, SubmenuVehSpawnerCompacts);
				sub->AddOption<SubOption>("Coupes", nullptr, SubmenuVehSpawnerCoupes);
				sub->AddOption<SubOption>("Motorcycles", nullptr, SubmenuVehSpawnerMotorcycles);
				sub->AddOption<SubOption>("Off-Road", nullptr, SubmenuVehSpawnerOffRoad);
				sub->AddOption<SubOption>("Cycles", nullptr, SubmenuVehSpawnerCycles);
				sub->AddOption<SubOption>("Vans", nullptr, SubmenuVehSpawnerVans);
				sub->AddOption<SubOption>("Ultility", nullptr, SubmenuVehSpawnerUltility);
				sub->AddOption<SubOption>("Industrial", nullptr, SubmenuVehSpawnerIndustrial);
				sub->AddOption<SubOption>("Service", nullptr, SubmenuVehSpawnerService);
				sub->AddOption<SubOption>("Commercial", nullptr, SubmenuVehSpawnerCommercial);
				sub->AddOption<SubOption>("Emergency", nullptr, SubmenuVehSpawnerEmergency);
				sub->AddOption<SubOption>("Military", nullptr, SubmenuVehSpawnerMilitary);
				sub->AddOption<SubOption>("Boats", nullptr, SubmenuVehSpawnerBoats);
				sub->AddOption<SubOption>("Planes", nullptr, SubmenuVehSpawnerPlanes);
				sub->AddOption<SubOption>("Helicopters", nullptr, SubmenuVehSpawnerHelicopters);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Spawner Settings", SubmenuVehSpawnerSettings, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Spawn Invincible", "Vehicle Will Be Spawned With Godmode", &features::vehicle_godmode, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn Inside", "You Will Be Spawned Inside Vehicle", &features::in_vehicle, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn Max Tuned", "Vehicle Will HAve Max Tuning", &features::full_stats, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn With Blip", "You Will See The Vehicle On Map", &features::vehicle_blip, BoolDisplay::OnOff);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sports", SubmenuVehSpawnerSports, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Alpha", "Spawn This Vehicle", [] {features::spawn_veh(0x2DB8D1AA); });
				//sub->AddOption<RegularOption>("Banshee", "Spawn This Vehicle", [] {features::spawn_veh(0xC1E908D2); });//Invalid vehicle model
				sub->AddOption<RegularOption>("BestiaGTS", "Spawn This Vehicle", [] {features::spawn_veh(0x4BFCF28B); });
				sub->AddOption<RegularOption>("Blista 2", "Spawn This Vehicle", [] {features::spawn_veh(0x3DEE5EDA); });
				sub->AddOption<RegularOption>("Blista 3", "Spawn This Vehicle", [] {features::spawn_veh(0xDCBC1C3B); });
				sub->AddOption<RegularOption>("Buffalo", "Spawn This Vehicle", [] {features::spawn_veh(0xEDD516C6); });
				sub->AddOption<RegularOption>("Buffalo 2", "Spawn This Vehicle", [] {features::spawn_veh(0x2BEC3CBE); });
				sub->AddOption<RegularOption>("Buffalo 3", "Spawn This Vehicle", [] {features::spawn_veh(0xE2C013E); });
				sub->AddOption<RegularOption>("Carbonizzare", "Spawn This Vehicle", [] {features::spawn_veh(0x7B8AB45F); });
				sub->AddOption<RegularOption>("Comet", "Spawn This Vehicle", [] {features::spawn_veh(0xC1AE4D16); });
				sub->AddOption<RegularOption>("Comet 2", "Spawn This Vehicle", [] {features::spawn_veh(0x877358AD); });
				sub->AddOption<RegularOption>("Comet 3", "Spawn This Vehicle", [] {features::spawn_veh(0x5D1903F9); });
				sub->AddOption<RegularOption>("Comet 4", "Spawn This Vehicle", [] {features::spawn_veh(0x276D98A3); });
				sub->AddOption<RegularOption>("Coquette", "Spawn This Vehicle", [] {features::spawn_veh(0x67BC037); });
				sub->AddOption<RegularOption>("Coquette (New)", "Spawn This Vehicle", [] {features::spawn_veh(0x98F65A5E); });
				//sub->AddOption<RegularOption>("Drafter", "Spawn This Vehicle", [] {features::spawn_veh(0x28EA﻿B80F); });
				sub->AddOption<RegularOption>("Deveste", "Spawn This Vehicle", [] {features::spawn_veh(0x5EE005DA); });
				sub->AddOption<RegularOption>("Elegy", "Spawn This Vehicle", [] {features::spawn_veh(0xBBA2261); });
				sub->AddOption<RegularOption>("Elegy 2", "Spawn This Vehicle", [] {features::spawn_veh(0xDE3D9D22); });
				sub->AddOption<RegularOption>("Feltzer", "Spawn This Vehicle", [] {features::spawn_veh(0x8911B9F5); });
				sub->AddOption<RegularOption>("Flash GT", "Spawn This Vehicle", [] {features::spawn_veh(0xB4F32118); });
				sub->AddOption<RegularOption>("Furore GT", "Spawn This Vehicle", [] {features::spawn_veh(0xBF1691E0); });
				sub->AddOption<RegularOption>("Fusilade", "Spawn This Vehicle", [] {features::spawn_veh(0x1DC0BA53); });
				sub->AddOption<RegularOption>("Futo", "Spawn This Vehicle", [] {features::spawn_veh(0x7836CE2F); });
				sub->AddOption<RegularOption>("GB 200", "Spawn This Vehicle", [] {features::spawn_veh(0x71CBEA98); });
				sub->AddOption<RegularOption>("Hotring", "Spawn This Vehicle", [] {features::spawn_veh(0x42836BE5); });
				sub->AddOption<RegularOption>("Komoda", "Spawn This Vehicle", [] {features::spawn_veh(0xCE44C4B9); });
				sub->AddOption<RegularOption>("Imorgon", "Spawn This Vehicle", [] {features::spawn_veh(0xBC7C0A00); });
				sub->AddOption<RegularOption>("Issi 7", "Spawn This Vehicle", [] {features::spawn_veh(0x6E8DA4F7); });
				sub->AddOption<RegularOption>("Itali GTO", "Spawn This Vehicle", [] {features::spawn_veh(0xEC3E3404); });
				sub->AddOption<RegularOption>("Jugular", "Spawn This Vehicle", [] {features::spawn_veh(0xF38C4245); });
				sub->AddOption<RegularOption>("Jester", "Spawn This Vehicle", [] {features::spawn_veh(0xB2A716A3); });
				sub->AddOption<RegularOption>("Jester 2", "Spawn This Vehicle", [] {features::spawn_veh(0xBE0E6126); });
				sub->AddOption<RegularOption>("Jester 3", "Spawn This Vehicle", [] {features::spawn_veh(0xF330CB6A); });
				sub->AddOption<RegularOption>("Khamelion", "Spawn This Vehicle", [] {features::spawn_veh(0x206D1B68); });
				sub->AddOption<RegularOption>("Kuruma", "Spawn This Vehicle", [] {features::spawn_veh(0xAE2BFE94); });
				sub->AddOption<RegularOption>("Kuruma (Armored)", "Spawn This Vehicle", [] {features::spawn_veh(0x187D938D); });
				//sub->AddOption<RegularOption>("Locust", "Spawn This Vehicle", [] {features::spawn_veh(0xC7E55211﻿); });
				sub->AddOption<RegularOption>("Lynx", "Spawn This Vehicle", [] {features::spawn_veh(0x1CBDC10B); });
				sub->AddOption<RegularOption>("Massacro", "Spawn This Vehicle", [] {features::spawn_veh(0xF77ADE32); });
				sub->AddOption<RegularOption>("Massacro 2", "Spawn This Vehicle", [] {features::spawn_veh(0xDA5819A3); });
				//sub->AddOption<RegularOption>("Neo", "Spawn This Vehicle", [] {features::spawn_veh(0x9F6ED5A2﻿); });
				//sub->AddOption<RegularOption>("Neon", "Spawn This Vehicle", [] {features::spawn_veh(0x91CA96EE﻿); });
				sub->AddOption<RegularOption>("9F", "Spawn This Vehicle", [] {features::spawn_veh(0x3D8FA25C); });
				sub->AddOption<RegularOption>("9F v2", "Spawn This Vehicle", [] {features::spawn_veh(0xA8E38B01); });
				sub->AddOption<RegularOption>("Omnis", "Spawn This Vehicle", [] {features::spawn_veh(0xD1AD4937); });
				sub->AddOption<RegularOption>("(RIP) Paragon ", "Spawn This Vehicle", [] {features::spawn_veh(0xE550775B); });
				sub->AddOption<RegularOption>("(RIP) Paragon (Armored)", "Spawn This Vehicle", [] {features::spawn_veh(0x546D8EEE); });
				sub->AddOption<RegularOption>("Pariah", "Spawn This Vehicle", [] {features::spawn_veh(0x33B98FE2); });
				sub->AddOption<RegularOption>("Penumbra", "Spawn This Vehicle", [] {features::spawn_veh(0xE9805550); });
				sub->AddOption<RegularOption>("Penumbra 2", "Spawn This Vehicle", [] {features::spawn_veh(0xDA5EC7DA); });
				sub->AddOption<RegularOption>("Raiden", "Spawn This Vehicle", [] {features::spawn_veh(0xA4D99B7D); });
				sub->AddOption<RegularOption>("Rapid GT", "Spawn This Vehicle", [] {features::spawn_veh(0x8CB29A14); });
				sub->AddOption<RegularOption>("Rapid GT 2", "Spawn This Vehicle", [] {features::spawn_veh(0x679450AF); });
				sub->AddOption<RegularOption>("Raptor", "Spawn This Vehicle", [] {features::spawn_veh(0xD7C56D39); });
				sub->AddOption<RegularOption>("Revolter", "Spawn This Vehicle", [] {features::spawn_veh(0xE78CC3D9); });
				sub->AddOption<RegularOption>("Ruston", "Spawn This Vehicle", [] {features::spawn_veh(0x2AE524A8); });
				sub->AddOption<RegularOption>("Schafter 2", "Spawn This Vehicle", [] {features::spawn_veh(0xB52B5113); });
				sub->AddOption<RegularOption>("Schafter 3", "Spawn This Vehicle", [] {features::spawn_veh(0xA774B5A6); });
				sub->AddOption<RegularOption>("Schafter 4", "Spawn This Vehicle", [] {features::spawn_veh(0x58CF185C); });
				sub->AddOption<RegularOption>("Schafter 5", "Spawn This Vehicle", [] {features::spawn_veh(0xCB0E7CD9); });
				sub->AddOption<RegularOption>("Schafter 6", "Spawn This Vehicle", [] {features::spawn_veh(0x72934BE4); });
				sub->AddOption<RegularOption>("Schlagen", "Spawn This Vehicle", [] {features::spawn_veh(0xE1C03AB0); });
				sub->AddOption<RegularOption>("Schwarzer", "Spawn This Vehicle", [] {features::spawn_veh(0xD37B7976); });
				sub->AddOption<RegularOption>("Sentinel", "Spawn This Vehicle", [] {features::spawn_veh(0x41D149AA); });
				sub->AddOption<RegularOption>("Seven", "Spawn This Vehicle", [] {features::spawn_veh(0x97398A4B); });
				sub->AddOption<RegularOption>("Specter", "Spawn This Vehicle", [] {features::spawn_veh(0x706E2B40); });
				sub->AddOption<RegularOption>("Specter 2", "Spawn This Vehicle", [] {features::spawn_veh(0x400F5147); });
				sub->AddOption<RegularOption>("Streiter", "Spawn This Vehicle", [] {features::spawn_veh(0x67D2B389); });
				sub->AddOption<RegularOption>("Sugoi", "Spawn This Vehicle", [] {features::spawn_veh(0x3ADB9758); });
				sub->AddOption<RegularOption>("Sultan", "Spawn This Vehicle", [] {features::spawn_veh(0x39DA2754); });
				sub->AddOption<RegularOption>("Sultan 2", "Spawn This Vehicle", [] {features::spawn_veh(0x3404691C); });
				sub->AddOption<RegularOption>("Surano", "Spawn This Vehicle", [] {features::spawn_veh(0x16E478C1); });
				sub->AddOption<RegularOption>("Tampa", "Spawn This Vehicle", [] {features::spawn_veh(0xC0240885); });
				sub->AddOption<RegularOption>("Tropos", "Spawn This Vehicle", [] {features::spawn_veh(0x707E63A4); });
				sub->AddOption<RegularOption>("Verlierer", "Spawn This Vehicle", [] {features::spawn_veh(0x41B77FA4); });
				sub->AddOption<RegularOption>("VSTR", "Spawn This Vehicle", [] {features::spawn_veh(0x56CDEE7D); });
				sub->AddOption<RegularOption>("ZR 380", "Spawn This Vehicle", [] {features::spawn_veh(0x20314B42); });
				sub->AddOption<RegularOption>("ZR 380 (Armored)", "Spawn This Vehicle", [] {features::spawn_veh(0xBE11EFC6); });
				sub->AddOption<RegularOption>("ZR 3803", "Spawn This Vehicle", [] {features::spawn_veh(0xA7DCC35C); });
				sub->AddOption<RegularOption>("Itali RSX", "Spawn This Vehicle", [] {features::spawn_veh(0xBB78956A); });
				sub->AddOption<RegularOption>("Veto", "Spawn This Vehicle", [] {features::spawn_veh(0xCCE5C8FA); });
				sub->AddOption<RegularOption>("Veto 2", "Spawn This Vehicle", [] {features::spawn_veh(0xA703E4A9); });
				sub->AddOption<RegularOption>("ZR 350", "Spawn This Vehicle", [] {features::spawn_veh(0x91373058); });
				sub->AddOption<RegularOption>("Calico", "Spawn This Vehicle", [] {features::spawn_veh(0xB8D657AD); });
				sub->AddOption<RegularOption>("Futo 2", "Spawn This Vehicle", [] {features::spawn_veh(0xA6297CC8); });
				sub->AddOption<RegularOption>("Euros", "Spawn This Vehicle", [] {features::spawn_veh(0x7980BDD5); });
				sub->AddOption<RegularOption>("Jester", "Spawn This Vehicle", [] {features::spawn_veh(0xA1B3A871); });
				sub->AddOption<RegularOption>("Remus", "Spawn This Vehicle", [] {features::spawn_veh(0x5216AD5E); });
				sub->AddOption<RegularOption>("Comet 6", "Spawn This Vehicle", [] {features::spawn_veh(0x991EFC04); });
				sub->AddOption<RegularOption>("Growler", "Spawn This Vehicle", [] {features::spawn_veh(0x4DC079D7); });
				sub->AddOption<RegularOption>("Vectre", "Spawn This Vehicle", [] {features::spawn_veh(0xA42FC3A5); });
				sub->AddOption<RegularOption>("Cypher", "Spawn This Vehicle", [] {features::spawn_veh(0x68A5D1EF); });
				sub->AddOption<RegularOption>("Sultan 3", "Spawn This Vehicle", [] {features::spawn_veh(0xEEA75E63); });
				sub->AddOption<RegularOption>("RT 3000", "Spawn This Vehicle", [] {features::spawn_veh(0xE505CF99); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Muscle", SubmenuVehSpawnerMuscle, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Blade", "Spawn This Vehicle", [] {features::spawn_veh(0xB820ED5E); });
				sub->AddOption<RegularOption>("Buccaneer", "Spawn This Vehicle", [] {features::spawn_veh(0xD756460C); });
				sub->AddOption<RegularOption>("Buccaneer 2", "Spawn This Vehicle", [] {features::spawn_veh(0xC397F748); });
				sub->AddOption<RegularOption>("Chino", "Spawn This Vehicle", [] {features::spawn_veh(0x14D69010); });
				sub->AddOption<RegularOption>("Chino 2", "Spawn This Vehicle", [] {features::spawn_veh(0xAED64A63); });
				sub->AddOption<RegularOption>("Clique", "Spawn This Vehicle", [] {features::spawn_veh(0xA29F78B0); });
				sub->AddOption<RegularOption>("Coquette 3", "Spawn This Vehicle", [] {features::spawn_veh(0x2EC385FE); });
				sub->AddOption<RegularOption>("Deviant", "Spawn This Vehicle", [] {features::spawn_veh(0x4C3FFF49); });
				sub->AddOption<RegularOption>("Dominator", "Spawn This Vehicle", [] {features::spawn_veh(0x4CE68AC); });
				sub->AddOption<RegularOption>("Dominator 2", "Spawn This Vehicle", [] {features::spawn_veh(0xC96B73D9); });
				sub->AddOption<RegularOption>("Dominator 3", "Spawn This Vehicle", [] {features::spawn_veh(0xC52C6B93); });
				sub->AddOption<RegularOption>("Dominator 4", "Spawn This Vehicle", [] {features::spawn_veh(0xD6FB0F30); });
				sub->AddOption<RegularOption>("Dominator 5", "Spawn This Vehicle", [] {features::spawn_veh(0xAE0A3D4F); });
				sub->AddOption<RegularOption>("Dominator 6", "Spawn This Vehicle", [] {features::spawn_veh(0xB2E046FB); });
				sub->AddOption<RegularOption>("Dukes", "Spawn This Vehicle", [] {features::spawn_veh(0x2B26F456); });
				sub->AddOption<RegularOption>("Dukes 2", "Spawn This Vehicle", [] {features::spawn_veh(0xEC8F7094); });
				sub->AddOption<RegularOption>("Dukes 3", "Spawn This Vehicle", [] {features::spawn_veh(0x7F3415E3); });
				sub->AddOption<RegularOption>("Faction", "Spawn This Vehicle", [] {features::spawn_veh(0x81A9CDDF); });
				sub->AddOption<RegularOption>("Faction 2", "Spawn This Vehicle", [] {features::spawn_veh(0x95466BDB); });
				sub->AddOption<RegularOption>("Faction 3", "Spawn This Vehicle", [] {features::spawn_veh(0x866BCE26); });
				sub->AddOption<RegularOption>("Ellie", "Spawn This Vehicle", [] {features::spawn_veh(0xB472D2B5); });
				sub->AddOption<RegularOption>("Gauntlet", "Spawn This Vehicle", [] {features::spawn_veh(0x94B395C5); });
				sub->AddOption<RegularOption>("Gauntlet 2", "Spawn This Vehicle", [] {features::spawn_veh(0x14D22159); });
				sub->AddOption<RegularOption>("Gauntlet 3", "Spawn This Vehicle", [] {features::spawn_veh(0x2B0C4DCD); });
				sub->AddOption<RegularOption>("Gauntlet 4", "Spawn This Vehicle", [] {features::spawn_veh(0x734C5E50); });
				sub->AddOption<RegularOption>("Gauntlet 5", "Spawn This Vehicle", [] {features::spawn_veh(0x817AFAAD); });
				sub->AddOption<RegularOption>("Hermes", "Spawn This Vehicle", [] {features::spawn_veh(0xE83C17); });
				sub->AddOption<RegularOption>("Hotknife", "Spawn This Vehicle", [] {features::spawn_veh(0x239E390); });
				sub->AddOption<RegularOption>("Hustler", "Spawn This Vehicle", [] {features::spawn_veh(0x23CA25F2); });
				sub->AddOption<RegularOption>("Impaler", "Spawn This Vehicle", [] {features::spawn_veh(0x83070B62); });
				sub->AddOption<RegularOption>("Impaler 2", "Spawn This Vehicle", [] {features::spawn_veh(0x3C26BD0C); });
				sub->AddOption<RegularOption>("Impaler 3", "Spawn This Vehicle", [] {features::spawn_veh(0x8D45DF49); });
				sub->AddOption<RegularOption>("Impaler 4", "Spawn This Vehicle", [] {features::spawn_veh(0x9804F4C7); });
				sub->AddOption<RegularOption>("Imperator", "Spawn This Vehicle", [] {features::spawn_veh(0x1A861243); });
				sub->AddOption<RegularOption>("Imperator 2", "Spawn This Vehicle", [] {features::spawn_veh(0x619C1B82); });
				sub->AddOption<RegularOption>("Imperator 3", "Spawn This Vehicle", [] {features::spawn_veh(0xD2F77E37); });
				sub->AddOption<RegularOption>("Lurcher", "Spawn This Vehicle", [] {features::spawn_veh(0x7B47A6A7); });
				sub->AddOption<RegularOption>("Moonbeam", "Spawn This Vehicle", [] {features::spawn_veh(0x1F52A43F); });
				sub->AddOption<RegularOption>("Moonbeam 2", "Spawn This Vehicle", [] {features::spawn_veh(0x710A2B9B); });
				sub->AddOption<RegularOption>("Nightshade", "Spawn This Vehicle", [] {features::spawn_veh(0x8C2BD0DC); });
				sub->AddOption<RegularOption>("Peyote 2", "Spawn This Vehicle", [] {features::spawn_veh(0x9472CD24); });
				sub->AddOption<RegularOption>("Phoenix", "Spawn This Vehicle", [] {features::spawn_veh(0x831A21D5); });
				sub->AddOption<RegularOption>("Picador", "Spawn This Vehicle", [] {features::spawn_veh(0x59E0FBF3); });
				sub->AddOption<RegularOption>("Ratloader", "Spawn This Vehicle", [] {features::spawn_veh(0xD83C13CE); });
				sub->AddOption<RegularOption>("Ratloader 2", "Spawn This Vehicle", [] {features::spawn_veh(0xDCE1D9F7); });
				sub->AddOption<RegularOption>("Ruiner", "Spawn This Vehicle", [] {features::spawn_veh(0xF26CEFF9); });
				sub->AddOption<RegularOption>("Ruiner 2", "Spawn This Vehicle", [] {features::spawn_veh(0x381E10BD); });
				sub->AddOption<RegularOption>("Ruined Ruiner", "Spawn This Vehicle", [] {features::spawn_veh(0x2E5AFD37); });
				sub->AddOption<RegularOption>("Sabre GT", "Spawn This Vehicle", [] {features::spawn_veh(0x9B909C94); });
				sub->AddOption<RegularOption>("Sabre GT 2", "Spawn This Vehicle", [] {features::spawn_veh(0xD4EA603); });
				sub->AddOption<RegularOption>("Slamvan", "Spawn This Vehicle", [] {features::spawn_veh(0x2B7F9DE3); });
				sub->AddOption<RegularOption>("Slamvan 2", "Spawn This Vehicle", [] {features::spawn_veh(0x31ADBBFC); });
				sub->AddOption<RegularOption>("Slamvan 3", "Spawn This Vehicle", [] {features::spawn_veh(0x42BC5E19); });
				sub->AddOption<RegularOption>("Slamvan 4", "Spawn This Vehicle", [] {features::spawn_veh(0x8526E2F5); });
				sub->AddOption<RegularOption>("Slamvan 5", "Spawn This Vehicle", [] {features::spawn_veh(0x163F8520); });
				sub->AddOption<RegularOption>("Slamvan 6", "Spawn This Vehicle", [] {features::spawn_veh(0x67D52852); });
				sub->AddOption<RegularOption>("Stalion", "Spawn This Vehicle", [] {features::spawn_veh(0x72A4C31E); });
				sub->AddOption<RegularOption>("Stalion 2", "Spawn This Vehicle", [] {features::spawn_veh(0xE80F67EE); });
				sub->AddOption<RegularOption>("Tampa", "Spawn This Vehicle", [] {features::spawn_veh(0x39F9C898); });
				sub->AddOption<RegularOption>("Tampa 3", "Spawn This Vehicle", [] {features::spawn_veh(0xB7D9F7F1); });
				sub->AddOption<RegularOption>("Tulip", "Spawn This Vehicle", [] {features::spawn_veh(0x56D42971); });
				sub->AddOption<RegularOption>("Vamos", "Spawn This Vehicle", [] {features::spawn_veh(0xFD128DFD); });
				sub->AddOption<RegularOption>("Vigero", "Spawn This Vehicle", [] {features::spawn_veh(0xCEC6B9B7); });
				sub->AddOption<RegularOption>("Virgo", "Spawn This Vehicle", [] {features::spawn_veh(0xE2504942); });
				sub->AddOption<RegularOption>("Virgo 2", "Spawn This Vehicle", [] {features::spawn_veh(0xCA62927A); });
				sub->AddOption<RegularOption>("Virgo 3", "Spawn This Vehicle", [] {features::spawn_veh(0xFDFFB0); });
				sub->AddOption<RegularOption>("Voodoo", "Spawn This Vehicle", [] {features::spawn_veh(0x779B4F2D); });
				sub->AddOption<RegularOption>("Voodoo 2", "Spawn This Vehicle", [] {features::spawn_veh(0x1F3766E3); });
				sub->AddOption<RegularOption>("Yosemite", "Spawn This Vehicle", [] {features::spawn_veh(0x6F946279); });
				sub->AddOption<RegularOption>("Yosemite 2", "Spawn This Vehicle", [] {features::spawn_veh(0x64F49967); });
				sub->AddOption<RegularOption>("Yosemite 3", "Spawn This Vehicle", [] {features::spawn_veh(0x0409D787); });
				sub->AddOption<RegularOption>("Dominator 7", "Spawn This Vehicle", [] {features::spawn_veh(0x196F9418); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Super", SubmenuVehSpawnerSuper, [&](RegularSubmenu* sub)
			{

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sport Classics", SubmenuVehSpawnerSportClassics, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sedans", SubmenuVehSpawnerSedans, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("SUVs", SubmenuVehSpawnerSUVs, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Compacts", SubmenuVehSpawnerCompacts, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Asbo", "Spawn This Vehicle", [] {features::spawn_veh(0x42ACA95F); });
				sub->AddOption<RegularOption>("Blista", "Spawn This Vehicle", [] {features::spawn_veh(0xEB70965F); });
				sub->AddOption<RegularOption>("Brioso", "Spawn This Vehicle", [] {features::spawn_veh(0x5C55CB39); });
				sub->AddOption<RegularOption>("Club", "Spawn This Vehicle", [] {features::spawn_veh(0x82E47E85); });
				sub->AddOption<RegularOption>("Dilettante", "Spawn This Vehicle", [] {features::spawn_veh(0xBC993509); });
				sub->AddOption<RegularOption>("Dilettante 2", "Spawn This Vehicle", [] {features::spawn_veh(0xBC993509); });
				sub->AddOption<RegularOption>("Kanjo", "Spawn This Vehicle", [] {features::spawn_veh(0x18619B7E); });
				sub->AddOption<RegularOption>("Issi", "Spawn This Vehicle", [] {features::spawn_veh(0xB9CB3B69); });
				sub->AddOption<RegularOption>("Issi 2", "Spawn This Vehicle", [] {features::spawn_veh(0x378236E1); });
				sub->AddOption<RegularOption>("Issi 3", "Spawn This Vehicle", [] {features::spawn_veh(0x256E92BA); });
				sub->AddOption<RegularOption>("Issi 4", "Spawn This Vehicle", [] {features::spawn_veh(0x5BA0FF1E); });
				sub->AddOption<RegularOption>("Issi 5", "Spawn This Vehicle", [] {features::spawn_veh(0x49E25BA1); });
				sub->AddOption<RegularOption>("Panto", "Spawn This Vehicle", [] {features::spawn_veh(0xE644E480); });
				sub->AddOption<RegularOption>("Prairie", "Spawn This Vehicle", [] {features::spawn_veh(0xE644E480); });
				sub->AddOption<RegularOption>("Rhapsody", "Spawn This Vehicle", [] {features::spawn_veh(0x322CF98F); });
				sub->AddOption<RegularOption>("Brioso 2", "Spawn This Vehicle", [] {features::spawn_veh(0x55365079); });
				sub->AddOption<RegularOption>("Weevil", "Spawn This Vehicle", [] {features::spawn_veh(0x61FE4D6A); });
				
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Coupes", SubmenuVehSpawnerCoupes, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Cogcabrio", "Spawn This Vehicle", [] {features::spawn_veh(0x13B57D8A); });
				sub->AddOption<RegularOption>("Exemplar", "Spawn This Vehicle", [] {features::spawn_veh(0xFFB15B5E); });
				sub->AddOption<RegularOption>("F620", "Spawn This Vehicle", [] {features::spawn_veh(0xDCBCBE48); });
				sub->AddOption<RegularOption>("Felon", "Spawn This Vehicle", [] {features::spawn_veh(0xE8A8BDA8); });
				sub->AddOption<RegularOption>("Felon 2", "Spawn This Vehicle", [] {features::spawn_veh(0xFAAD85EE); });
				sub->AddOption<RegularOption>("Jackal", "Spawn This Vehicle", [] {features::spawn_veh(0xDAC67112); });
				sub->AddOption<RegularOption>("Oracle", "Spawn This Vehicle", [] {features::spawn_veh(0x506434F6); });
				sub->AddOption<RegularOption>("Oracle 2", "Spawn This Vehicle", [] {features::spawn_veh(0xE18195B2); });
				sub->AddOption<RegularOption>("Sentinel", "Spawn This Vehicle", [] {features::spawn_veh(0x50732C82); });
				sub->AddOption<RegularOption>("Sentinel 2", "Spawn This Vehicle", [] {features::spawn_veh(0x3412AE2D); });
				sub->AddOption<RegularOption>("Windsor", "Spawn This Vehicle", [] {features::spawn_veh(0x5E4327C8); });
				sub->AddOption<RegularOption>("Windsor 2", "Spawn This Vehicle", [] {features::spawn_veh(0x8CF5CAE1); });
				sub->AddOption<RegularOption>("Zion", "Spawn This Vehicle", [] {features::spawn_veh(0xBD1B39C3); });
				sub->AddOption<RegularOption>("Zion 2", "Spawn This Vehicle", [] {features::spawn_veh(0xB8E2AE18); });
				sub->AddOption<RegularOption>("Previon", "Spawn This Vehicle", [] {features::spawn_veh(0x546DA331); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Motorcycles", SubmenuVehSpawnerMotorcycles, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Off-Road", SubmenuVehSpawnerOffRoad, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Cycles", SubmenuVehSpawnerCycles, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("BMX", "Spawn This Vehicle", [] {features::spawn_veh(0x43779C54); });
				sub->AddOption<RegularOption>("Cruiser", "Spawn This Vehicle", [] {features::spawn_veh(0x1ABA13B5); });
				sub->AddOption<RegularOption>("Fixter", "Spawn This Vehicle", [] {features::spawn_veh(0xCE23D3BF); });
				sub->AddOption<RegularOption>("Scorcher", "Spawn This Vehicle", [] {features::spawn_veh(0xF4E1AA15); });
				sub->AddOption<RegularOption>("Tribike", "Spawn This Vehicle", [] {features::spawn_veh(0x4339CD69); });
				sub->AddOption<RegularOption>("Tribike 2", "Spawn This Vehicle", [] {features::spawn_veh(0xB67597EC); });
				sub->AddOption<RegularOption>("Tribike 3", "Spawn This Vehicle", [] {features::spawn_veh(0xE823FB48); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Vans", SubmenuVehSpawnerVans, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Utility", SubmenuVehSpawnerUltility, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Industrial", SubmenuVehSpawnerIndustrial, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Bulldozer", "Spawn This Vehicle", [] {features::spawn_veh(0x7074F39D); });
				sub->AddOption<RegularOption>("Cutter", "Spawn This Vehicle", [] {features::spawn_veh(0xC3FBA120); });
				sub->AddOption<RegularOption>("Dump", "Spawn This Vehicle", [] {features::spawn_veh(0x810369E2); });
				sub->AddOption<RegularOption>("Flatbed", "Spawn This Vehicle", [] {features::spawn_veh(0x50B0215A); });
				sub->AddOption<RegularOption>("Guardian", "Spawn This Vehicle", [] {features::spawn_veh(0x825A9F4C); });
				sub->AddOption<RegularOption>("Handler", "Spawn This Vehicle", [] {features::spawn_veh(0x1A7FCEFA); });
				sub->AddOption<RegularOption>("Mixer", "Spawn This Vehicle", [] {features::spawn_veh(0xD138A6BB); });
				sub->AddOption<RegularOption>("Mixer 2", "Spawn This Vehicle", [] {features::spawn_veh(0x1C534995); });
				sub->AddOption<RegularOption>("Rubble", "Spawn This Vehicle", [] {features::spawn_veh(0x9A5B1DCC); });
				sub->AddOption<RegularOption>("Tiptruck", "Spawn This Vehicle", [] {features::spawn_veh(0x2E19879); });
				sub->AddOption<RegularOption>("Tiptruck 2", "Spawn This Vehicle", [] {features::spawn_veh(0xC7824E5E); });


			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sevice", SubmenuVehSpawnerService, [&](RegularSubmenu* sub)
			{
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Commercial", SubmenuVehSpawnerCommercial, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Benson", "Spawn This Vehicle", [] {features::spawn_veh(0x7A61B330); });
				sub->AddOption<RegularOption>("Biff", "Spawn This Vehicle", [] {features::spawn_veh(0x32B91AE8); });
				sub->AddOption<RegularOption>("Cerberus", "Spawn This Vehicle", [] {features::spawn_veh(0xD039510B); });
				sub->AddOption<RegularOption>("Cerberus 2", "Spawn This Vehicle", [] {features::spawn_veh(0x287FA449); });
				sub->AddOption<RegularOption>("Cerberus 3", "Spawn This Vehicle", [] {features::spawn_veh(0x71D3B6F0); });
				sub->AddOption<RegularOption>("Hauler", "Spawn This Vehicle", [] {features::spawn_veh(0x5A82F9AE); });
				sub->AddOption<RegularOption>("Hauler 2", "Spawn This Vehicle", [] {features::spawn_veh(0x171C92C4); });
				sub->AddOption<RegularOption>("Mule", "Spawn This Vehicle", [] {features::spawn_veh(0x35ED670B); });
				sub->AddOption<RegularOption>("Mule 2", "Spawn This Vehicle", [] {features::spawn_veh(0xC1632BEB); });
				sub->AddOption<RegularOption>("Mule 3", "Spawn This Vehicle", [] {features::spawn_veh(0x85A5B471); });
				sub->AddOption<RegularOption>("Mule 4", "Spawn This Vehicle", [] {features::spawn_veh(0x73F4110E); });
				sub->AddOption<RegularOption>("Packer", "Spawn This Vehicle", [] {features::spawn_veh(0x21EEE87D); });
				sub->AddOption<RegularOption>("Phantom", "Spawn This Vehicle", [] {features::spawn_veh(0x809AA4CB); });
				sub->AddOption<RegularOption>("Phantom (POG one)", "Spawn This Vehicle", [] {features::spawn_veh(0x9DAE1398); });
				sub->AddOption<RegularOption>("Phantom 3", "Spawn This Vehicle", [] {features::spawn_veh(0xA90ED5C); });
				sub->AddOption<RegularOption>("Pounder", "Spawn This Vehicle", [] {features::spawn_veh(0x7DE35E7D); });
				sub->AddOption<RegularOption>("Pounder 2", "Spawn This Vehicle", [] {features::spawn_veh(0x6290F15B); });
				sub->AddOption<RegularOption>("Stockade", "Spawn This Vehicle", [] {features::spawn_veh(0x6827CF72); });
				sub->AddOption<RegularOption>("Stockade 2", "Spawn This Vehicle", [] {features::spawn_veh(0xF337AB36); });
				sub->AddOption<RegularOption>("Terrorbyte", "Spawn This Vehicle", [] {features::spawn_veh(0x897AFC65); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Emergency", SubmenuVehSpawnerEmergency, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Ambulance", "Spawn This Vehicle", [] {features::spawn_veh(0x45D56ADA); });
				sub->AddOption<RegularOption>("FBI", "Spawn This Vehicle", [] {features::spawn_veh(0x432EA949); });
				sub->AddOption<RegularOption>("FBI 2", "Spawn This Vehicle", [] {features::spawn_veh(0x9DC66994); });
				sub->AddOption<RegularOption>("Firetruck", "Spawn This Vehicle", [] {features::spawn_veh(0x73920F8E); });
				sub->AddOption<RegularOption>("Lifeguard", "Spawn This Vehicle", [] {features::spawn_veh(0x1BF8D381); });
				sub->AddOption<RegularOption>("Prison bus", "Spawn This Vehicle", [] {features::spawn_veh(0x885F3671); });
				sub->AddOption<RegularOption>("Police Cruiser", "Spawn This Vehicle", [] {features::spawn_veh(0x79FBB0C5); });
				sub->AddOption<RegularOption>("Police 2", "Spawn This Vehicle", [] {features::spawn_veh(0x9F05F101); });
				sub->AddOption<RegularOption>("Police 3", "Spawn This Vehicle", [] {features::spawn_veh(0x71FA16EA); });
				sub->AddOption<RegularOption>("Police 4", "Spawn This Vehicle", [] {features::spawn_veh(0x8A63C7B9); });
				sub->AddOption<RegularOption>("Police Bike", "Spawn This Vehicle", [] {features::spawn_veh(0xFDEFAEC3); });
				sub->AddOption<RegularOption>("Maverick", "Spawn This Vehicle", [] {features::spawn_veh(0x1517D4D9); });
				sub->AddOption<RegularOption>("Police Snow", "Spawn This Vehicle", [] {features::spawn_veh(0xA46462F7); });
				sub->AddOption<RegularOption>("Police Snow 2", "Spawn This Vehicle", [] {features::spawn_veh(0x95F4C618); });
				sub->AddOption<RegularOption>("Police Van", "Spawn This Vehicle", [] {features::spawn_veh(0x1B38E955); });
				sub->AddOption<RegularOption>("Pranger", "Spawn This Vehicle", [] {features::spawn_veh(0x2C33B46E); });
				sub->AddOption<RegularOption>("Predator", "Spawn This Vehicle", [] {features::spawn_veh(0xE2E7D4AB); });
				sub->AddOption<RegularOption>("Riot", "Spawn This Vehicle", [] {features::spawn_veh(0xB822A1AA); });
				sub->AddOption<RegularOption>("Riot 2", "Spawn This Vehicle", [] {features::spawn_veh(0x9B16A3B4); });
				sub->AddOption<RegularOption>("Sheriff", "Spawn This Vehicle", [] {features::spawn_veh(0x9BAA707C); });
				sub->AddOption<RegularOption>("Sheriff 2", "Spawn This Vehicle", [] {features::spawn_veh(0x72935408); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Military", SubmenuVehSpawnerMilitary, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("APC", "Spawn This Vehicle", [] {features::spawn_veh(0x2189D250); });
				sub->AddOption<RegularOption>("Barracks", "Spawn This Vehicle", [] {features::spawn_veh(0xCEEA3F4B); });
				sub->AddOption<RegularOption>("Barracks 2", "Spawn This Vehicle", [] {features::spawn_veh(0x4008EABB); });
				sub->AddOption<RegularOption>("Barracks 3", "Spawn This Vehicle", [] {features::spawn_veh(0x2592B5CF); });
				sub->AddOption<RegularOption>("Barrage", "Spawn This Vehicle", [] {features::spawn_veh(0xF34DFB25); });
				sub->AddOption<RegularOption>("Chernobog", "Spawn This Vehicle", [] {features::spawn_veh(0xD6BC7523); });
				sub->AddOption<RegularOption>("Crusader", "Spawn This Vehicle", [] {features::spawn_veh(0x132D5A1A); });
				sub->AddOption<RegularOption>("Halftrack", "Spawn This Vehicle", [] {features::spawn_veh(0xFE141DA6); });
				sub->AddOption<RegularOption>("Khanjali", "Spawn This Vehicle", [] {features::spawn_veh(0xAA6F980A); });
				sub->AddOption<RegularOption>("Minitank", "Spawn This Vehicle", [] {features::spawn_veh(0xB53C6C52); });
				sub->AddOption<RegularOption>("Rhino", "Spawn This Vehicle", [] {features::spawn_veh(0x2EA68690); });
				sub->AddOption<RegularOption>("Scarab", "Spawn This Vehicle", [] {features::spawn_veh(0xBBA2A2F7); });
				sub->AddOption<RegularOption>("Scarab 2", "Spawn This Vehicle", [] {features::spawn_veh(0x5BEB3CE0); });
				sub->AddOption<RegularOption>("Scarab 3", "Spawn This Vehicle", [] {features::spawn_veh(0xDD71BFEB); });
				sub->AddOption<RegularOption>("Thruster", "Spawn This Vehicle", [] {features::spawn_veh(0x58CDAF30); });
				sub->AddOption<RegularOption>("Gun Trailer", "Spawn This Vehicle", [] {features::spawn_veh(0x8FD54EBB); });
				sub->AddOption<RegularOption>("Vetir", "Spawn This Vehicle", [] {features::spawn_veh(0x780FFBD2); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Boats", SubmenuVehSpawnerBoats, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Dinghy", "Spawn This Vehicle", [] {features::spawn_veh(0x3D961290); });
				sub->AddOption<RegularOption>("Dinghy 2", "Spawn This Vehicle", [] {features::spawn_veh(0x107F392C); });
				sub->AddOption<RegularOption>("Dinghy 3", "Spawn This Vehicle", [] {features::spawn_veh(0x1E5E54EA); });
				sub->AddOption<RegularOption>("Dinghy 4", "Spawn This Vehicle", [] {features::spawn_veh(0x33B47F96); });
				sub->AddOption<RegularOption>("Jetmax", "Spawn This Vehicle", [] {features::spawn_veh(0x33581161); });
				sub->AddOption<RegularOption>("Marquis", "Spawn This Vehicle", [] {features::spawn_veh(0xC1CE1183); });
				sub->AddOption<RegularOption>("Seashark", "Spawn This Vehicle", [] {features::spawn_veh(0xC2974024); });
				sub->AddOption<RegularOption>("Seashark 2", "Spawn This Vehicle", [] {features::spawn_veh(0xDB4388E4); });
				sub->AddOption<RegularOption>("Seashark 3", "Spawn This Vehicle", [] {features::spawn_veh(0xED762D49); });
				sub->AddOption<RegularOption>("Speeder", "Spawn This Vehicle", [] {features::spawn_veh(0xDC60D2B); });
				sub->AddOption<RegularOption>("Speeder 2", "Spawn This Vehicle", [] {features::spawn_veh(0x1A144F2A); });
				sub->AddOption<RegularOption>("Squalo", "Spawn This Vehicle", [] {features::spawn_veh(0x17DF5EC2); });
				sub->AddOption<RegularOption>("Submersible", "Spawn This Vehicle", [] {features::spawn_veh(0x2DFF622F); });
				sub->AddOption<RegularOption>("Submersible 2", "Spawn This Vehicle", [] {features::spawn_veh(0xC07107EE); });
				sub->AddOption<RegularOption>("Suntrap", "Spawn This Vehicle", [] {features::spawn_veh(0xEF2295C9); });
				sub->AddOption<RegularOption>("Toro", "Spawn This Vehicle", [] {features::spawn_veh(0x3FD5AA2F); });
				sub->AddOption<RegularOption>("Toro 2", "Spawn This Vehicle", [] {features::spawn_veh(0x362CAC6D); });
				sub->AddOption<RegularOption>("Tropic", "Spawn This Vehicle", [] {features::spawn_veh(0x1149422F); });
				sub->AddOption<RegularOption>("Tropic 2", "Spawn This Vehicle", [] {features::spawn_veh(0x56590FE9); });
				sub->AddOption<RegularOption>("Tug", "Spawn This Vehicle", [] {features::spawn_veh(0x82CAC433); });
				sub->AddOption<RegularOption>("Avisa", "Spawn This Vehicle", [] {features::spawn_veh(0x9A474B5E); });
				sub->AddOption<RegularOption>("Dinghy 5", "Spawn This Vehicle", [] {features::spawn_veh(0xC58DA34A); });
				sub->AddOption<RegularOption>("Kosatka", "Idk i Wouldnt Spawn It in Storymode", [] {features::spawn_veh(0x4FAF0D70); });
				sub->AddOption<RegularOption>("Longfin", "Spawn This Vehicle", [] {features::spawn_veh(0x6EF89CCC); });
				sub->AddOption<RegularOption>("Patrolboat", "Spawn This Vehicle", [] {features::spawn_veh(0xEF813606); });
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Planes", SubmenuVehSpawnerPlanes, [&](RegularSubmenu* sub)
			{

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Helicopters", SubmenuVehSpawnerHelicopters, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Akula", "Spawn This Vehicle", [] {features::spawn_veh(0x46699F47); });
				sub->AddOption<RegularOption>("Annihilator", "Spawn This Vehicle", [] {features::spawn_veh(0x31F0B376); });
				sub->AddOption<RegularOption>("Buzzard", "Spawn This Vehicle", [] {features::spawn_veh(0x2F03547B); });
				sub->AddOption<RegularOption>("Buzzard 2", "Spawn This Vehicle", [] {features::spawn_veh(0x2C75F0DD); });
				sub->AddOption<RegularOption>("Cargobob", "Spawn This Vehicle", [] {features::spawn_veh(0xFCFCB68B); });
				sub->AddOption<RegularOption>("Cargobob 2", "Spawn This Vehicle", [] {features::spawn_veh(0x60A7EA10); });
				sub->AddOption<RegularOption>("Cargobob 3", "Spawn This Vehicle", [] {features::spawn_veh(0x53174EEF); });
				sub->AddOption<RegularOption>("Cargobob 4", "Spawn This Vehicle", [] {features::spawn_veh(0x78BC1A3C); });
				sub->AddOption<RegularOption>("Frogger", "Spawn This Vehicle", [] {features::spawn_veh(0x2C634FBD); });
				sub->AddOption<RegularOption>("Frogger 2", "Spawn This Vehicle", [] {features::spawn_veh(0x742E9AC0); });
				sub->AddOption<RegularOption>("Havok", "Spawn This Vehicle", [] {features::spawn_veh(0x89BA59F5); });
				sub->AddOption<RegularOption>("Hunter", "Spawn This Vehicle", [] {features::spawn_veh(0xFD707EDE); });
				sub->AddOption<RegularOption>("Maverick", "Spawn This Vehicle", [] {features::spawn_veh(0x9D0450CA); });
				sub->AddOption<RegularOption>("Savage", "Spawn This Vehicle", [] {features::spawn_veh(0xFB133A17); });
				sub->AddOption<RegularOption>("Seasparrow", "Spawn This Vehicle", [] {features::spawn_veh(0xD4AE63D9); });
				sub->AddOption<RegularOption>("Skylift", "Spawn This Vehicle", [] {features::spawn_veh(0x3E48BF23); });
				sub->AddOption<RegularOption>("Supervolito", "Spawn This Vehicle", [] {features::spawn_veh(0x2A54C47D); });
				sub->AddOption<RegularOption>("Supervolito 2", "Spawn This Vehicle", [] {features::spawn_veh(0x9C5E5644); });
				sub->AddOption<RegularOption>("Swift", "Spawn This Vehicle", [] {features::spawn_veh(0xEBC24DF2); });
				sub->AddOption<RegularOption>("Swift Deluxe", "Spawn This Vehicle", [] {features::spawn_veh(0x4019CB4C); });
				sub->AddOption<RegularOption>("Valkyrie", "Spawn This Vehicle", [] {features::spawn_veh(0xA09E15FD); });
				sub->AddOption<RegularOption>("Valkyrie 2", "Spawn This Vehicle", [] {features::spawn_veh(0x5BFA5C4B); });
				sub->AddOption<RegularOption>("Volatus", "Spawn This Vehicle", [] {features::spawn_veh(0x920016F1); });
				sub->AddOption<RegularOption>("Annihilator 2", "Spawn This Vehicle", [] {features::spawn_veh(0x11962E49); });
				sub->AddOption<RegularOption>("Seasparrow 2", "Spawn This Vehicle", [] {features::spawn_veh(0x494752F7); });
				sub->AddOption<RegularOption>("Seasparrow 3", "Spawn This Vehicle", [] {features::spawn_veh(0x5F017E6B); });


			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Online", OnlineSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Players", "All players in session", SubmenuPlayerList);
				sub->AddOption<SubOption>("Session Info", "Displays Info About Session", SessionInfoSubmenu);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Session Info", SessionInfoSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Session Info", "Displays Session Info", &features::sessioninfo, BoolDisplay::OnOff);
				
				sub->AddOption<BoolOption<bool>>("Speedometer", "Displays Session Info", &features::speedometer, BoolDisplay::OnOff);
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Weapon Menu", Weaponz, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Give All Weapons", "Gives you all weapons", []
					{
						int Weapons[] = { 0x181410A1, 0xC78D71B4, 0x45CD9CF3, 0xDB26713A, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, 0x3813FC08,
								0xA2719263, 0x8BB05FD7, 0xF9DCBF2D, 0xD8DF3C3C, 0xDD5DF8D9, 0xDFE37640, 0x19044EE0, 0xCD274149, 0x94117305, 0x3813FC08, 0xBFE256D4, 0x88374054, 0x83839C4, 0xDC4DB296, 0xC1B3C3D1, 0xCB96392F, 0x97EA20B8, 0x2BE6766B, 0x0A3D4D34, 0xDB1AA450, 0xBD248B55, 0x555AF99A, 0xEF951FBB, 0x12E82D3D, 0x394F415C, 0xFAD1F1C9, 0x969C3D67, 0x84D6FAFD, 0x624FE830, 0xDBBD7280, 0xA914799, 0x6A6C02E0, 0x6D544C99, 0x63AB0442, 0x0781FE4A, 0xA0973D5E, 0xAB564B93, 0xBA45E8B8, 0xFBAB5776, 0x060EC506, 0xAF3696A1, 0xD7DBF707, 0x476BF155, 0xB62D1F67,0xF38A0747, 0x167C5572, 0x2C9CA024, 0xA991DAE8, 0xD6678401, 0x6AA85572
						};
						for (int i = 0; i < (sizeof(Weapons) / 4); i++) {
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), Weapons[i], 9999, 1);
						}
					});
				sub->AddOption<RegularOption>("Max Ammo", "Max Ammo", []
					{
						Hash Ammo;
						WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &Ammo, 1);
						WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), Ammo, 9999, 9999);
					});
				sub->AddOption<BoolOption<bool>>("Infinite ammo", "Your gun will never be empty", &features::infiniteammo, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Remove Weapons", "Remove Weapons", []
					{
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), true);
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("World_Options", WorldOptions, [](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Time", "Adjust the World Time", TimeSubmenu);
				sub->AddOption<SubOption>("Weather", "Adjust the World Time", WeatherSubmenu);
				sub->AddOption<SubOption>("Water", "Mess With Water", WaterSubmenu);
				sub->AddOption<SubOption>("Clear Area", "Select what to clear from you", ClearSubmenu);
				sub->AddOption<NumberOption<std::int32_t>>("World Gravity", nullptr, &features::worldgravity, 0, 3, 1, 3, true, "", "", [] {});
				sub->AddOption<RegularOption>("Set Gravity", "", []
					{
						MISC::SET_GRAVITY_LEVEL(features::worldgravity);
					});
				sub->AddOption<NumberOption<float>>("Rain Intensity", nullptr, &features::rainlevelint, 0, 1000, 10, 3, true, "", "", [] {});
				sub->AddOption<RegularOption>("Set Intensity", "", []
					{
						MISC::_SET_RAIN_FX_INTENSITY(features::rainlevelint);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Time", TimeSubmenu, [](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Stop Time", "Freezes Time In the World", &features::stoptime, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<int>>("Hours", "Might Not Work In Online!", &features::HoursTime, 0, 23, 1, 1, true, "", "", []
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, 0, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, 0, 0);
					});
				sub->AddOption<NumberOption<int>>("Minutes", "Might Not Work In Online!", &features::MinutesTime, 0, 59, 1, 1, true, "", "", []
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
					});
				sub->AddOption<RegularOption>("Add 1 Hour", "Adds 1 Hour to Current World Time", []
					{
						CLOCK::ADD_TO_CLOCK_TIME(1, 0, 0);
					});
				sub->AddOption<RegularOption>("Reduce 1 Hour", "Removes 1 Hour of Current World Time", []
					{
						CLOCK::ADD_TO_CLOCK_TIME(-1, 0, 0);
					});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Weather", WeatherSubmenu, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Extra Sunny", "Set Weather Extra Sunny", []
					{
						MISC::SET_OVERRIDE_WEATHER("EXTRASUNNY");
					});
				sub->AddOption<RegularOption>("Clean", "Set Weather Clean", []
					{
						MISC::SET_OVERRIDE_WEATHER("CLEAR");
					});
				sub->AddOption<RegularOption>("Clearing", "Set Weather Clearing", []
					{
						MISC::SET_OVERRIDE_WEATHER("CLEARING");
					});
				sub->AddOption<RegularOption>("Cloudy", "Set Weather Cloudy", []
					{
						MISC::SET_OVERRIDE_WEATHER("CLOUDS");
					});
				sub->AddOption<RegularOption>("Smog", "Set Weather Smog", []
					{
						MISC::SET_OVERRIDE_WEATHER("SMOG");
					});
				sub->AddOption<RegularOption>("Foggy", "Set Weather Foggy", []
					{
						MISC::SET_OVERRIDE_WEATHER("FOGGY");
					});
				sub->AddOption<RegularOption>("Overcast", "Set Weather Overcast", []
					{
						MISC::SET_OVERRIDE_WEATHER("OVERCAST");
					});
				sub->AddOption<RegularOption>("Rain", "Set Weather Rain", []
					{
						MISC::SET_OVERRIDE_WEATHER("RAIN");
					});
				sub->AddOption<RegularOption>("Thunder", "Set Weather Thunder", []
					{
						MISC::SET_OVERRIDE_WEATHER("THUNDER");
					});
				sub->AddOption<RegularOption>("Neutral", "Set Weather Neutral", []
					{
						MISC::SET_OVERRIDE_WEATHER("NEUTRAL");
					});
				sub->AddOption<RegularOption>("Snow", "Set Weather Snow", []
					{
						MISC::SET_OVERRIDE_WEATHER("SNOW");
					});
				sub->AddOption<RegularOption>("Blizzard", "Set Weather Blizzard", []
					{
						MISC::SET_OVERRIDE_WEATHER("BLIZZARD");
					});
				sub->AddOption<RegularOption>("Light Snow", "Set Weather Light Snow", []
					{
						MISC::SET_OVERRIDE_WEATHER("SNOWLIGHT");
					});
				sub->AddOption<RegularOption>("Xmas", "Set Weather Xmas", []
					{
						MISC::SET_OVERRIDE_WEATHER("XMAS");
					});
				sub->AddOption<RegularOption>("Halloween", "Set Weather Halloween", []
					{
						MISC::SET_OVERRIDE_WEATHER("HALLOWEEN");
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Water", WaterSubmenu, [](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("No Water Mode", "Deletes Water", &features::nowater, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<int>>("Hours", "Might Not Work In Online!", &features::HoursTime, 0, 23, 1, 1, true, "", "", []
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, 0, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, 0, 0);
					});
				sub->AddOption<NumberOption<int>>("Minutes", "Might Not Work In Online!", &features::MinutesTime, 0, 59, 1, 1, true, "", "", []
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
					});
				sub->AddOption<RegularOption>("Add 1 Hour", "Adds 1 Hour to Current World Time", []
					{
						CLOCK::ADD_TO_CLOCK_TIME(1, 0, 0);
					});
				sub->AddOption<RegularOption>("Reduce 1 Hour", "Removes 1 Hour of Current World Time", []
					{
						CLOCK::ADD_TO_CLOCK_TIME(-1, 0, 0);
					});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Clear Area", ClearSubmenu, [](RegularSubmenu* sub)
			{

				sub->AddOption<NumberOption<int>>("Radius", "How far should objects be cleared?", &features::clearradius, 1, 500, 10, 1, true, "", "", []
					{
						NULL;
					});
				sub->AddOption<RegularOption>("Clear From Everything", "Deletes All Objects", []
					{
						MISC::_CLEAR_AREA_OF_EVERYTHING(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, true, true, true, true);
					});
				sub->AddOption<RegularOption>("Delete Vehicles", "Deletes All Vehicles In Area", []
					{
						MISC::CLEAR_AREA_OF_VEHICLES(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, true, true, true, true, true, NULL);
					});
				sub->AddOption<RegularOption>("Delete Peds", "Deletes All Peds In Area", []
					{
						MISC::CLEAR_AREA_OF_PEDS(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, 1);
					});
				sub->AddOption<RegularOption>("Delete Cops", "Deletes All Cops In Area", []
					{
						MISC::CLEAR_AREA_OF_COPS(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, 1);
					});

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Teleport Menu", teleports, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Teleport To Waypoint ", "Teleport to Waypoint", []
					{
						{
							Vector3 coords;
							Entity e = PLAYER::PLAYER_PED_ID();
							if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
								e = PED::GET_VEHICLE_PED_IS_USING(e);

							int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8);
							if (HUD::DOES_BLIP_EXIST(WaypointHandle))
							{
								Vector3 WaypointPos = HUD::GET_BLIP_COORDS(WaypointHandle);
								WaypointPos.z += 5.0f;
								int Handle = PLAYER::PLAYER_PED_ID();
								if (PED::IS_PED_IN_ANY_VEHICLE(Handle, 0))
									Handle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
								ENTITY::SET_ENTITY_COORDS(Handle, WaypointPos.x, WaypointPos.y, WaypointPos.z, 0, 0, 0, 1);
							}
						}
					});
				sub->AddOption<SubOption>("Airport's", nullptr, air);
				sub->AddOption<SubOption>("IPL's", nullptr, ipls);
				sub->AddOption<SubOption>("Landmarks", nullptr, landmarks);
				sub->AddOption<SubOption>("Garages", nullptr, gar);
				sub->AddOption<SubOption>("Story Safe Houses", nullptr, safehouse);
				sub->AddOption<SubOption>("Shops", nullptr, shops);

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Garage", gar, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("LSC", "Teleport to LSC", []
					{
						Vector3 Coords;
						Coords.x = -373.01; Coords.y = -124.91; Coords.z = 38.31;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Bennys Garage", "Teleport to Bennys Garage", []
					{
						//PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -205.3417f, -1305.824f, 31.3527f);
						teleport(-205.3417f, -1305.824f, 31.3527f);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Safe Houses", safehouse, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Franklins House", "Teleport to Franklins House", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), 7.1190f, 536.6150f, 176.0280f);
					});
				sub->AddOption<RegularOption>("Franklins House 2", "Teleport To Franklins House 2", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -14.3803f, -1483.510f, 31.1044f);
					});
				sub->AddOption<RegularOption>("Michaels House", "Teleport To Michaels House", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -14.3803f, -1483.510f, 31.1044f);
					});
				sub->AddOption<RegularOption>("Trevors House", "Teleport To Trevors House", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), 1972.6100f, 3817.0400f, 33.4283f);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Shops", shops, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Barber Shop", "Teleport To Barber Shop", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -1293.231f, -1117.002f, 6.6402f);
					});
				sub->AddOption<RegularOption>("Tattoo Shop", "Teleport To Tattoo Shop", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), 319.7877f, 172.2311f, 103.7454f);
					});
				sub->AddOption<RegularOption>("Clothes Shop", "Teleport To Clothes Shop", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -718.91, -158.16, 37.00);
					});
				sub->AddOption<RegularOption>("Gun Shop", "Teleport To Gun Shop", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), 247.3652, -45.8777, 69.9411);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Airports", air, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Airport LSIA ", "Teleport To airpot LSIA", []
					{
						Vector3 Coords;
						Coords.x = -1102.2910f; Coords.y = -2894.5160f; Coords.z = 13.9467f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Airport Sandy Shores ", "Teleport To Sandy Shores", []
					{
						Vector3 Coords;
						Coords.x = 1741.4960f; Coords.y = 3269.2570f; Coords.z = 41.6014f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("IPls", ipls, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("North Yankton ", "Prologue Vibes", []
					{
						STREAMING::REQUEST_IPL("prologue01");
						STREAMING::REQUEST_IPL("Prologue01c");
						STREAMING::REQUEST_IPL("Prologue01d");
						STREAMING::REQUEST_IPL("Prologue01e");
						STREAMING::REQUEST_IPL("Prologue01f");
						STREAMING::REQUEST_IPL("Prologue01g");
						STREAMING::REQUEST_IPL("prologue01h");
						STREAMING::REQUEST_IPL("prologue01i");
						STREAMING::REQUEST_IPL("prologue01j");
						STREAMING::REQUEST_IPL("prologue01k");
						STREAMING::REQUEST_IPL("prologue01z");
						STREAMING::REQUEST_IPL("prologue02");
						STREAMING::REQUEST_IPL("prologue03");
						STREAMING::REQUEST_IPL("prologue03b");
						STREAMING::REQUEST_IPL("prologue03_grv_cov");
						STREAMING::REQUEST_IPL("prologue03_grv_dug");
						STREAMING::REQUEST_IPL("prologue03_grv_fun");
						STREAMING::REQUEST_IPL("prologue04");
						STREAMING::REQUEST_IPL("prologue04b");
						STREAMING::REQUEST_IPL("prologue04_cover");
						STREAMING::REQUEST_IPL("prologue05");
						STREAMING::REQUEST_IPL("prologue05b");
						STREAMING::REQUEST_IPL("prologue06");
						STREAMING::REQUEST_IPL("prologue06b");
						STREAMING::REQUEST_IPL("prologue06_int");
						STREAMING::REQUEST_IPL("prologuerd");
						STREAMING::REQUEST_IPL("prologuerdb");
						STREAMING::REQUEST_IPL("prologue_DistantLights");
						STREAMING::REQUEST_IPL("prologue_grv_torch");
						STREAMING::REQUEST_IPL("prologue_m2_door");
						STREAMING::REQUEST_IPL("prologue_LODLights");
						STREAMING::REQUEST_IPL("DES_ProTree_start");
						Vector3 Coords;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), 3595.39673f, -4893.727f, 115.838394f);
					});
				sub->AddOption<RegularOption>("Porn Yacht ", "woo", []
					{
						STREAMING::REQUEST_IPL("smboat");
						Vector3 Coords;
						Coords.x = -2045.8f; Coords.y = -1031.2f; Coords.z = 11.9f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -2045.8f, -1031.2f, 11.9f);

					});
				sub->AddOption<RegularOption>("Life Invader Office", "Teleport To Life Invader Office", []
					{
						STREAMING::REQUEST_IPL("facelobby");
						STREAMING::REQUEST_IPL("facelobbyfake");
						Vector3 Coords;
						Coords.x = -1047.9f; Coords.y = -233.0f; Coords.z = 39.0f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -1047.9f, -233.0f, 39.0f);

					});
				sub->AddOption<RegularOption>("Cargoship", "Teleport To Cargoship", []
					{
						STREAMING::REQUEST_IPL("cargoship");
						Vector3 Coords;
						Coords.x = -162.8918f; Coords.y = -2365.769f; Coords.z = 9.3192f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);

					});
				sub->AddOption<RegularOption>("Oneil Farm", "Teleport To Oneil Farm", []
					{
						STREAMING::REMOVE_IPL("farm_burnt");
						STREAMING::REMOVE_IPL("farm_burnt_props");
						STREAMING::REQUEST_IPL("farm");
						STREAMING::REQUEST_IPL("farm_props");
						STREAMING::REQUEST_IPL("farmint");
						STREAMING::REQUEST_IPL("farmint_cap");
						Vector3 Coords;
						Coords.x = 2441.2f; Coords.y = 4968.5f; Coords.z = 51.7f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);

					});
				sub->AddOption<RegularOption>("Hotspital ", "Teleport To hotspital", []
					{
						STREAMING::REQUEST_IPL("RC12B_HospitalInterior");
						STREAMING::REQUEST_IPL("RC12B_Destroyed");
						Vector3 Coords;
						Coords.x = 356.8f; Coords.y = -590.1f; Coords.z = 43.3f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
						;
					});
				sub->AddOption<RegularOption>("Jewelry Store", "Teleport To Jewelry Store", []
					{
						STREAMING::REQUEST_IPL("jewel2fake");
						STREAMING::REQUEST_IPL("post_hiest_unload");
						STREAMING::REQUEST_IPL("bh1_16_refurb");
						Vector3 Coords;
						Coords.x = -630.4f; Coords.y = -236.7f; Coords.z = 40.0f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);

					});
				sub->AddOption<RegularOption>("Morgue", "Teleport To Morgue", []
					{
						STREAMING::REQUEST_IPL("Coroner_Int_on");
						Vector3 Coords;
						Coords.x = 244.9f; Coords.y = -1374.7f; Coords.z = 39.5f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Landmarks", landmarks, [](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Crane Top", "Teleport To Crane Top", []
					{
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -119.8790f, -977.3570f, 304.2491f);
					});
				sub->AddOption<RegularOption>("Mount Chiliad", "Teleport To Mount Chilliad", []
					{
						Vector3 Coords;
						Coords.x = 496.75f; Coords.y = 5591.17f; Coords.z = 795.03f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Maze Bank", "Teleport To Maze Bank", []
					{
						Vector3 Coords;
						Coords.x = -74.94243f; Coords.y = -818.63446f; Coords.z = 326.174347f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Military Base", "Teleport To Military Base", []
					{
						Vector3 Coords;
						Coords.x = -2012.8470f; Coords.y = 2956.5270f; Coords.z = 32.8101f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Zancudo Tower", "Teleport To Zancudo Tower", []
					{
						Vector3 Coords;
						Coords.x = -2356.0940; Coords.y = 3248.645; Coords.z = 101.4505;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Mask Shop", "Teleport To Mask Shop", []
					{
						Vector3 Coords;
						Coords.x = -1338.16; Coords.y = -1278.11; Coords.z = 4.87;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("LSC", "Teleport To LSC", []
					{
						Vector3 Coords;
						Coords.x = -373.01; Coords.y = -124.91; Coords.z = 38.31;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("FBI", "Teleport To FBI", []
					{
						Vector3 Coords;
						Coords.x = 135.5220f; Coords.y = -749.0003f; Coords.z = 260.0000f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Human Labs", "Teleport To Human Labs", []
					{
						Vector3 Coords;
						Coords.x = 3617.231f; Coords.y = 3739.871f; Coords.z = 28.6901f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("MMI", "Teleport To MMI", []
					{
						Vector3 Coords;
						Coords.x = -222.1977; Coords.y = -1185.8500; Coords.z = 23.0294;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Waterfall", "Teleport To Waterfall", []
					{
						Vector3 Coords;
						Coords.x = -597.9525f; Coords.y = 4475.2910f; Coords.z = 25.6890f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
			});


		g_UiManager->AddSubmenu<RegularSubmenu>("Players", SubmenuPlayerList, [](RegularSubmenu* sub)
			{
				for (std::uint32_t i = 0; i < 32; ++i)
				{
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
					{
						if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == i)
						{
							char Buffer[255]; sprintf(Buffer, "%s   ~p~[SESSION HOST]", PLAYER::GET_PLAYER_NAME(i));  /////// HOST 
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									//NULL = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == PLAYER::PLAYER_PED_ID())
						{
							char Buffer[255]; sprintf(Buffer, "%s   ~b~[SELF]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									//NULL = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == NETWORK::NETWORK_IS_FRIEND_IN_MULTIPLAYER(PLAYER::GET_PLAYER_NAME(i))) {
							char Buffer[255]; sprintf(Buffer, "%s   ~g~[FRIEND]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									//NULL = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == NETWORK::NETWORK_IS_IN_SPECTATOR_MODE()) {
							char Buffer[255]; sprintf(Buffer, "%s   ~b~[SPECTATING]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									//NULL = i;
								});
						}
						// 
						else
						{
							sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [=]
								{
									//NULL = i;
								});
						}
					}
				}
			});
		//GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.25, 0.25, 0.25, 200, 94, 100, 255, 1, 1, 1, 0, 0, 0, 0);

		//g_UiManager->AddSubmenu<PlayerSubmenu>(features::g_selected_player, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
		//	{
		//		sub->AddOption<BoolOption<bool>>("Spectate", nullptr, &features::spectateplayer, BoolDisplay::OnOff);
		//		sub->AddOption<SubOption>("Teleport Options", nullptr, SelectedPlayerTeleport);
		//		sub->AddOption<SubOption>("Abuse", nullptr, SelectedPlayerAbuse);
		//		sub->AddOption<SubOption>("Trolling Options", nullptr, SelectedPlayerTrolling);
		//		sub->AddOption<SubOption>("Friendly Options", nullptr, SelectedPlayerFriendly);

		//		g_UiManager->AddSubmenu<RegularSubmenu>("Player Teleports", SelectedPlayerTeleport, [](RegularSubmenu* sub)
		//			{
		//				sub->AddOption<RegularOption>("Teleport To Player", "tp to Player", []
		//					{
		//						Entity handle;
		//						PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
		//						ENTITY::SET_ENTITY_COORDS(handle, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false).z, false, false, false, false);
		//					});
		//				sub->AddOption<RegularOption>("Teleport To Player's Vehicle", "tp to Players vehicle", []
		//					{
		//						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(NULL), true))
		//						{
		//							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(NULL)), VEHICLE::IS_VEHICLE_SEAT_FREE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(NULL)), -1, true));
		//						}
		//					});
		//			});

		//		g_UiManager->AddSubmenu<RegularSubmenu>("Abuse", SelectedPlayerAbuse, [](RegularSubmenu* sub)
		//			{
		//				if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == PLAYER::PLAYER_PED_ID())
		//				{
		//					sub->AddOption<RegularOption>("Host Kick", "Kick From Session", []
		//						{
		//							NETWORK::NETWORK_SESSION_KICK_PLAYER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL));
		//							int hostkick[69] = { 1885259, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL) };
		//							SCRIPT::TRIGGER_SCRIPT_EVENT(1, hostkick, 4, 1 << NULL);
		//							//1885259 host kick global
		//						});
		//				}
		//				
		//				sub->AddOption<RegularOption>("Crash", "Trap In cage", []
		//					{
		//						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), true);
		//						Object crashobject = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("v_ilev_chair02_ped"), pos.x, pos.y, pos.z, 1, 1, 1);

		//						
		//					});
		//				sub->AddOption<RegularOption>("Non Host Kick", "Kick From Session", []
		//					{

		//						/*std::uint64_t args_6[] = { 1770262894, NULL, 0, 0 };
		//						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args_6, 4, NULL);*/
		//					});
		//				sub->AddOption<RegularOption>("Test Crash", "Test Crash From Session", []
		//					{
		//						/*std::uint64_t args_5[] = { -1386010354, NULL, 0, 0 };
		//						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args_5, 4, 1 << NULL);*/

		//					});
		//				sub->AddOption<RegularOption>("Script Kick", "Kick From Session", []
		//					{
		//						/*std::uint64_t args[] = { -145306724, NULL, 0, 0 };
		//						SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 4, 1 << NULL);*/

		//						
		//					});
		//			});

		//		g_UiManager->AddSubmenu<RegularSubmenu>("Trolling", SelectedPlayerTrolling, [](RegularSubmenu* sub)
		//			{
		//				sub->AddOption<RegularOption>("Clone Player", "Clones Player", []
		//					{
		//						PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), 1, 1, 1);
		//					});
		//				sub->AddOption<RegularOption>("Attach To Player", "Attaches You To Player", []
		//					{
		//						if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL) == PLAYER::PLAYER_PED_ID())
		//						{

		//						}
		//						else
		//						{
		//							ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, false, true);
		//						}
		//					});
		//				sub->AddOption<RegularOption>("Detach Off Player", "Detach You From Player", []
		//					{
		//						ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), true, true);
		//					});
		//				sub->AddOption<RegularOption>("Explode Player", "Blow Up Player", []
		//					{
		//						Vector3 targetCords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false);
		//						FIRE::ADD_EXPLOSION(targetCords.x, targetCords.y, targetCords.z, 29, 9999.0f, true, false, 0.0f, false);
		//					});
		//				sub->AddOption<RegularOption>("Airstrike Player", "Blow Up Player With Airstrike", []
		//					{
		//						if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL) == PLAYER::PLAYER_PED_ID())
		//						{
		//							NULL;
		//						}
		//						else
		//						{
		//							Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
		//							Ped playerPed = PLAYER::PLAYER_PED_ID();
		//							Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
		//							Hash airStrike = RAGE_JOAAT("WEAPON_AIRSTRIKE_ROCKET");
		//							WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
		//							while (!WEAPON::HAS_WEAPON_ASSET_LOADED(airStrike)) {}
		//							MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 50.f, coords.x, coords.y, coords.z, 250, 1, airStrike, playerPed, 1, 0, -1.0);
		//						}
		//					});
		//				sub->AddOption<RegularOption>("Cage Player", "Trap In cage", []
		//					{
		//						OBJECT::CREATE_OBJECT(959275690, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), false).z - 1.f, true, false, false);
		//					});
		//				sub->AddOption<RegularOption>("Tube Player", "Trap In Tube", []
		//					{
		//						Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), 0);
		//						Object obj = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("stt_prop_stunt_tube_fn_01"), remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, true);
		//						ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 90.0f, 0.0f, 0, true);
		//					});
		//				sub->AddOption<RegularOption>("Tree Player", "Trap In Tree", []
		//					{
		//						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), true);
		//						Object obj1 = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("prop_xmas_tree_int"), pos.x, pos.y, pos.z, 1, 1, 1);
		//					});

		//			});

		//		g_UiManager->AddSubmenu<RegularSubmenu>("Friendly", SelectedPlayerFriendly, [](RegularSubmenu* sub)
		//			{

		//			});





		//		sub->AddOption<RegularOption>("Set Waypoint", "tp to Players vehicle", []
		//			{
		//				std::int32_t sp = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
		//				if (ENTITY::DOES_ENTITY_EXIST(sp)) {
		//					Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(sp, 0.0f, 0.0f, 0.0f);
		//					HUD::SET_NEW_WAYPOINT(coords.x, coords.y);
		//				}
		//			});

		//		sub->AddOption<RegularOption>("Copy Outfit", "Steal Players Outfit", []
		//			{
		//				for (int i = 0; i < 12; i++)
		//				{
		//					PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), i), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), i), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), i));
		//					for (int i = 0; i < 2; i++) {
		//					}
		//				}
		//			});

		//	});
		g_UiManager->AddSubmenu<RegularSubmenu>("Miscellaneous", misc, [](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Mobile_Radio", "Vibe to the music everywhere!", &features::mobileradio, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Disable_Phone", "English Dave wont bother you", &features::nophone, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Skip Cutscene", "Skips current cutscene", []
					{
						
						CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
					});

				sub->AddOption<RegularOption>("Break Session test", "test", []
					{
						MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("Freemode");
						//
					});
			});


		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [](RegularSubmenu* sub)
		{
				sub->AddOption<RegularOption>("Switch GUI", "Switches to regular UI.", []
					{
						g_list = false;
						g_gui.m_opened = true;
					});
				sub->AddOption<RegularOption>("Unload", "Unload the menu.", []
					{
						g_running = false;
					});
			sub->AddOption<SubOption>("Header", nullptr, SubmenuSettingsHeader);
			sub->AddOption<SubOption>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
			sub->AddOption<SubOption>("Options", nullptr, SubmenuSettingsOption);
			sub->AddOption<SubOption>("Footer", nullptr, SubmenuSettingsFooter);
			sub->AddOption<SubOption>("Description", nullptr, SubmenuSettingsDescription);
			sub->AddOption<SubOption>("Input", nullptr, SubmenuSettingsInput);
			sub->AddOption<SubOption>("Language", nullptr, SubmenuSettingsLanguage);
			sub->AddOption<NumberOption<float>>("X Position", nullptr, &g_UiManager->m_PosX, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("Y Position", nullptr, &g_UiManager->m_PosY, 0.f, 1.f, 0.01f, 2);
			sub->AddOption<NumberOption<float>>("Width", nullptr, &g_UiManager->m_Width, 0.01f, 1.f, 0.01f, 2);
			sub->AddOption<BoolOption<bool>>("Sounds", nullptr, &g_UiManager->m_Sounds, BoolDisplay::OnOff);	
		});
	
		g_UiManager->AddSubmenu<RegularSubmenu>("Infobar", SubmenuSettingsSubmenuBar, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_SubmenuBarHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", nullptr, &g_UiManager->m_SubmenuBarTextSize, 0.01f, 1.f, 0.01f, 2);

			sub->AddOption<NumberOption<std::uint8_t>>("Background R", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background G", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background B", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background A", nullptr, &g_UiManager->m_SubmenuBarBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text R", nullptr, &g_UiManager->m_SubmenuBarTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text G", nullptr, &g_UiManager->m_SubmenuBarTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text B", nullptr, &g_UiManager->m_SubmenuBarTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Text A", nullptr, &g_UiManager->m_SubmenuBarTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Options", SubmenuSettingsOption, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_OptionHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", nullptr, &g_UiManager->m_OptionTextSize, 0.01f, 1.f, 0.01f, 2);

			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background R", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background G", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background B", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Background A", nullptr, &g_UiManager->m_OptionSelectedBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background R", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background G", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background B", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Background A", nullptr, &g_UiManager->m_OptionUnselectedBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));

			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text R", nullptr, &g_UiManager->m_OptionSelectedTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text G", nullptr, &g_UiManager->m_OptionSelectedTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text B", nullptr, &g_UiManager->m_OptionSelectedTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Selected Text A", nullptr, &g_UiManager->m_OptionSelectedTextColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text R", nullptr, &g_UiManager->m_OptionUnselectedTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text G", nullptr, &g_UiManager->m_OptionUnselectedTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text B", nullptr, &g_UiManager->m_OptionUnselectedTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Unselected Text A", nullptr, &g_UiManager->m_OptionUnselectedTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Footer", SubmenuSettingsFooter, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_FooterHeight, 0.01f, 0.1f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Sprite Size", nullptr, &g_UiManager->m_FooterSpriteSize, 0.01f, 1.f, 0.001f, 3);

			sub->AddOption<NumberOption<std::uint8_t>>("Background R", nullptr, &g_UiManager->m_FooterBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background G", nullptr, &g_UiManager->m_FooterBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background B", nullptr, &g_UiManager->m_FooterBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Background A", nullptr, &g_UiManager->m_FooterBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite R", nullptr, &g_UiManager->m_FooterSpriteColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite G", nullptr, &g_UiManager->m_FooterSpriteColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite B", nullptr, &g_UiManager->m_FooterSpriteColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("Sprite A", nullptr, &g_UiManager->m_FooterSpriteColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header", SubmenuSettingsHeader, [](RegularSubmenu* sub)
		{
			sub->AddOption<ChooseOption<const char*, std::size_t>>("Type", nullptr, &Lists::HeaderTypesFrontend, &Lists::HeaderTypesPosition, true, []
			{
				g_UiManager->m_HeaderType = Lists::HeaderTypesBackend[Lists::HeaderTypesPosition];
			});

			switch (g_UiManager->m_HeaderType)
			{
			case HeaderType::Static:
				sub->AddOption<SubOption>("Background", nullptr, SubmenuSettingsHeaderStaticBackground);
				break;
			case HeaderType::Gradient:
				sub->AddOption<SubOption>("Gradient", nullptr, SubmenuSettingsHeaderGradientBackground);
				break;
			}

			sub->AddOption<SubOption>("Text", nullptr, SubmenuSettingsHeaderText);
			sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_HeaderHeight, 0.01f, 0.2f, 0.001f, 3);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Background", SubmenuSettingsHeaderStaticBackground, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Gradient", SubmenuSettingsHeaderGradientBackground, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Transparent", nullptr, &g_UiManager->m_HeaderGradientTransparent, BoolDisplay::YesNo);
			sub->AddOption<BoolOption<bool>>("Flip", nullptr, &g_UiManager->m_HeaderGradientFlip, BoolDisplay::YesNo);

			sub->AddOption<NumberOption<std::uint8_t>>("R1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A1", nullptr, &g_UiManager->m_HeaderGradientColorLeft.a, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("R2", nullptr, &g_UiManager->m_HeaderGradientColorRight.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G2", nullptr, &g_UiManager->m_HeaderGradientColorRight.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B2", nullptr, &g_UiManager->m_HeaderGradientColorRight.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A2", nullptr, &g_UiManager->m_HeaderGradientColorRight.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Text", SubmenuSettingsHeaderText, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_UiManager->m_HeaderText, BoolDisplay::OnOff);
			sub->AddOption<NumberOption<float>>("Size", nullptr, &g_UiManager->m_HeaderTextSize, 0.1f, 2.f, 0.01f, 2);
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Description", SubmenuSettingsDescription, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<float>>("Padding", "Padding before the description rect.", &g_UiManager->m_DescriptionHeightPadding, 0.01f, 1.f, 0.001f,
 3);
			sub->AddOption<NumberOption<float>>("Height", "Size of the description rect.", &g_UiManager->m_DescriptionHeight, 0.01f, 1.f, 0.001f, 3);
			sub->AddOption<NumberOption<float>>("Text Size", "Size of the description text.", &g_UiManager->m_DescriptionTextSize, 0.1f, 2.f, 0.01f, 2);
		});

		g_UiManager->AddSubmenu<RegularSubmenu>("Input", SubmenuSettingsInput, [](RegularSubmenu* sub)
		{
			sub->AddOption<NumberOption<std::int32_t>>("Open Delay", nullptr, &g_UiManager->m_OpenDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Back Delay", nullptr, &g_UiManager->m_BackDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Enter Delay", nullptr, &g_UiManager->m_EnterDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Vertical Delay", nullptr, &g_UiManager->m_VerticalDelay, 10, 1000, 10, 0);
			sub->AddOption<NumberOption<std::int32_t>>("Horizontal Delay", nullptr, &g_UiManager->m_HorizontalDelay, 10, 1000, 10, 0);
		});

		/*g_UiManager->AddSubmenu<RegularSubmenu>("Players", SubmenuPlayerList, [](RegularSubmenu* sub)
		{
			for (std::uint32_t i = 0; i < 32; ++i)
			{
				if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
				{
					sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [=]
					{
						features::g_selected_player = i;
					});
				}
			}
		});

		g_UiManager->AddSubmenu<PlayerSubmenu>(&features::g_selected_player, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
		{
			sub;
		});*/
	}

	void MainScript::script_on_tick()
	{
		TRY_CLAUSE
		{
			if (g_UiManager->m_Opened)
			{
				PAD::DISABLE_CONTROL_ACTION(0, 27, true);
			}
		}
		EXCEPT_CLAUSE
	}

	void MainScript::script_func()
	{
		while (true)
		{
			TRY_CLAUSE
			{
				g_MainScript.script_init();
				g_MainScript.script_on_tick();
				g_MainScript.gui_on_tick();
			}
			EXCEPT_CLAUSE
			script::get_current()->yield();
		}
	}
}
