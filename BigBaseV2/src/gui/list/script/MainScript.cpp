#include "MainScript.hpp"
#include "../Lists.hpp"
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
#include "helpers/other.h"
#include "Translation.hpp"
#include "thread_pool.hpp"
#include "helpers/other.h"
#include <looped/teleports.hpp>
#include "looped/gtaData.hpp"
#include <gui/player_list.h>
#include <gui/list/BreakOption.hpp>
#pragma comment(lib, "Winmm.lib")




namespace zyko
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
		SubmenuVehicleAcrobatics,
		SubmenuNetwork,
		SubmenuSpawner,
		vis,
		SubmenuVehicle,
		SubmenuVehicleSpawner,
		SubmenuWeapons,
		SubmenuWorld,
		SubmenuTeleport,
		SubmenuMiscellaneous,
		SubmenuSettingsObjectSpanwer,
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
		AllPlayerAbuse,
		AllPlayerFriendly,
		AllPlayerTeleport,
		AllPlayerTrolling,
		recovery,
		SubmenuVehicleMovement,
		SubmenuModelChanger,
		misc,
		ProtectionsSubmenu,
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
		SessionJoinerSubmenu,
		SubmenuSettingsParticles,
		SessionAllPlayersSubmenu,
		SubmenuVehSpawnerSettings,
		SubmenuVehSpawnerSports,
		SubmenuVehSpawnerMuscle,
		SubmenuVehSpawnerSuper,
		SUbmenuSettingsLinks,
		SubmenuVehSpawnerSportClassics,
		SubmenuVehSpawnerSedans,
		SubmenuVehSpawnerSUVs,
		SubmenuVehSpawnerCompacts,
		SubmenuVehSpawnerCoupes,
		SubmenuVehSpawnerMotorcycles,
		SubmenuVehSpawnerOffRoad,
		SubmenuVehSpawnerCycles,
		SubmenuVehSpawnerVans,
		SubmenuVehSpawnerUltility,
		SubmenuVehSpawnerIndustrial,
		SubmenuVehSpawnerService,
		SubmenuVehSpawnerCommercial,
		SubmenuVehSpawnerEmergency,
		SubmenuVehSpawnerMilitary,
		SubmenuVehSpawnerBoats,
		SubmenuVehSpawnerPlanes,
		SubmenuVehSpawnerHelicopters,
		SelectedPlayerVehicle,
		AllPlayerVehicle,
		SessionSettingsSubmenu,
		RockstarAdminSubmenu,
		MoneySubmenu,
		LevelSubmenu,
		HeistSubmenu,
		scripthook,
		AnimationsSubmenu,
		SettingsInfoWindows,
		AllPlayersESP,
		SelectedPlayerESP

	};

	void MainScript::gui_init()
	{
		register_ytd();
	}

	void MainScript::gui_on_tick()
	{
		g_UiManager->OnTick();
	}

	void MainScript::script_init()
	{
		m_Initialized = true;
		using namespace UserInterface;

		if (g_list)
		{
			
		
		g_UiManager->AddSubmenu<RegularSubmenu>("Home", SubmenuHome, [&](RegularSubmenu* sub)
			{

				/*sub->AddOption<ChooseOption<const char*, std::size_t>>("test", nullptr, &Lists::HeaderTypesFrontend, &Lists::HeaderTypesPosition, true, [&]
					{
						
					});*/
				sub->AddOption<SubOption>("Self", "Self Options", SubmenuSelf);
				sub->AddOption<SubOption>("Vehicle Options", "Vehicle Options", SubmenuVehicle);
				sub->AddOption<SubOption>("Weapons", "Weapon Options", Weaponz);
				sub->AddOption<SubOption>("Online", "Online Options", OnlineSubmenu);
				sub->AddOption<SubOption>("Teleport", "Teleport Options", teleports);
				sub->AddOption<SubOption>("World Options", "World Options", WorldOptions);
				sub->AddOption<SubOption>("Recovery", "Recovery Options", recovery);
				sub->AddOption<SubOption>("Misc Options", "Other Options", misc);
				sub->AddOption<SubOption>("Protections", "Protection Options", ProtectionsSubmenu);
				//sub->AddOption<SubOption>("Scripthook V", "SHV Mods", scripthook);
				sub->AddOption<SubOption>("Settings", "Menu settings", SubmenuSettings);

			});

		g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Self Options"), SubmenuSelf, [&](RegularSubmenu* sub)
			{
				/*sub->AddOption<RegularOption>("Coords", "Nigger", [&]
					{

						const char* nigger = features::Keyboard().c_str();
						VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), nigger);

					});*/
				
				/*sub->AddOption<RegularOption>("Coords", "Nigger", [&]
					{
					
						Vector3 nigger = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
						float x = nigger.x;
						float y = nigger.y;
						float z = nigger.z;
						LOG(INFO) << x;
						LOG(INFO) << y;
						LOG(INFO) << z;


					});*/
				sub->AddOption<SubOption>("Visions", "Change Player Visions", vis);
				sub->AddOption<SubOption>("Model Changer", "Change your Model", SubmenuModelChanger);
				sub->AddOption<SubOption>("Animations", nullptr, AnimationsSubmenu);
				sub->AddOption<BreakOption>("Test", "Test");
				

				sub->AddOption<BoolOption<bool>>("God Mode", "You Cannot Die", &features::godmode, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Semi God Mode", "Cant Be Detected But Doesnt Work Against Explosions", &features::semigod, BoolDisplay::OnOff);

				sub->AddOption<BoolOption<bool>>("Noclip", "WASD - Direction | SPACE - Up | Shift - Down", &features::noclip, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Never Wanted", "Police Wont Start Coming After You", &features::neverwanted, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Instantly Enter Vehicles", "Come On Lets GO!!!", &features::instantenter, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<int>>("Wanted Level", nullptr, &features::wantedLevel, 0, 5, 1, 3, true, "< ", " >", [&]
					{
						PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
						PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
					});
				sub->AddOption<BoolOption<bool>>("Ignore Player", "Everyone will ignore you", &features::ignoreplayer, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Super Run - Override", "Run Boiiiiiii", &features::superrunbool, BoolDisplay::OnOff);
				if (features::superrunbool) {
					sub->AddOption<NumberOption<float>>("Run Speed", nullptr, &features::runspeed, 0.1, 5.0, 0.1, 3, true, "< ", " >", [&] {});
				}
				sub->AddOption<BoolOption<bool>>("Super Run - Force", "Run Boiiiiiii", &features::ultrarunbool, BoolDisplay::OnOff);
				if (features::ultrarunbool) {
					sub->AddOption<NumberOption<float>>("Run Speed", nullptr, &features::runspeed1, 0.1, 5.0, 0.1, 3, true, "< ", " >", [&] {});
				}
				sub->AddOption<BoolOption<bool>>("No Ragdoll", "You are invisible", &features::norag, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Flash Mode", "Run Like Flash", &features::flashrun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Super Jump", "Ever wanted to jump higher?", &features::superjumpbool, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Ultra Jump", "Jump Really High", &features::ultrajumpbool, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Forcefield", "Push everyone", &features::forcefield, BoolDisplay::OnOff);
				if(features::forcefield){ sub->AddOption<NumberOption<float>>("Force", nullptr, &features::forcefield_force, 0, 20, 1, 1, true, "< ", " >", [&] {}); }

				sub->AddOption<BoolOption<bool>>("Super Man", "Just Fly", &features::superman, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Off Radar", "Players wont see you on minimap", &features::offradar, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Bullshark Testosterone", "Players wont see you on minimap", &features::bullshark, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Modify Time Scale", "Speed Of The Whole World", &features::modifytimecycle, BoolDisplay::OnOff);
				if (features::modifytimecycle)
				{
					sub->AddOption<NumberOption<float>>("Time Scale", "You Can Do Slow Motion", &features::timescale, 0.0, 1.0, 0.1, 1, true, "< ", " >", [&] {

						});
				}
				sub->AddOption<RegularOption>("Clear Wanted", "Clear Player Wanted Level", [&]
					{
						PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
					});

				sub->AddOption<RegularOption>("Max Health", "Adds Health To The Player", [&]
					{
						features::maxhealth();
					});
				sub->AddOption<RegularOption>("Max Armour", "Adds Armour To The Player", [&]{features::maxarmor();});
				sub->AddOption<BoolOption<bool>>("Invisibility", "You are invisible", &features::invisibility, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<float>>("Ped Wetness", nullptr, &features::set_wetness, -1.0f, 1.0f, 0.1, 1, true, "< ", " >", [&] {
					features::Set_wetness(features::set_wetness);
					});
				sub->AddOption<NumberOption<int>>("Player Opacity", "Visibility Of Your Player", &features::playeralpha, 0, 255, 1, 3, true, "< ", " >", [&] {
					ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), features::playeralpha, 0);
					});
				sub->AddOption<BoolOption<bool>>("Infinite Stamina", "You Can Just Run Forever", &features::unlimitedstamina, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clean Ped", "Clean Player", [&]
					{
						PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
					});
				sub->AddOption<RegularOption>("Randomise Clothes", "Randomises Players Clothes", [&]
					{
						PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), true);
					});
				sub->AddOption<RegularOption>("Reset Ped", "Reset Player", [&] {features::resetped();});
				sub->AddOption<RegularOption>("Suicide", "Kills Ped", [&]{features::suicide();});
				/*sub->AddOption<RegularOption>(xorstr_("RegularOption_demo"), "A regular option.", [&]
					{
						LOG(INFO) << "Test Option pressed";
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
				sub->AddOption<ChooseOption<std::uint64_t, std::size_t>>("Vector", nullptr, &vector, &vectorPos);*/
			});
			g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Model Changer"), SubmenuModelChanger, [&](RegularSubmenu* sub)
				{
					for(auto& model: PedModels)
					{ 
						sub->AddOption<RegularOption>(model, "Set this model", [model]
							{
								features::changemodel(model);
							});
					}
					
				});
		g_UiManager->AddSubmenu<RegularSubmenu>("Visions", vis, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Night Vision", "You can see cool in night", &features::nightvision, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Thermal Vision", "Run Boiiiiiii", &features::thermalvision, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clear", "Clears Vision", [&]
					{
						GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
					});
				for (const char* modifiers : TimecycleModifiers)
				{
					sub->AddOption<RegularOption>(modifiers, "Set this modifier", [=]
						{
							GRAPHICS::SET_TIMECYCLE_MODIFIER(modifiers);							
						});
				}	
			});

			g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Animations"), AnimationsSubmenu, [&](RegularSubmenu* sub)
				{
					sub->AddOption<RegularOption>("test", "", [&]
						{
											//TASK::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "amb@code_human_wander_drinking@male@base", "	static", 1.f, 1.f, 1500, 1, 1, 1, 1, 1);
							features::changemodel("A_C_Boar");
						});

				});
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Movement", SubmenuVehicleMovement, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Bypass Max Speed", "Go faster then GTA allows", &features::speedbypass, BoolDisplay::OnOff);

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
						sub->AddOption<NumberOption<int>>("Horn Boost Speed", nullptr, &features::hornboostvalue, 0, 200, 10, 3, true, "< ", " >", [&] {});
					}
					sub->AddOption<BoolOption<bool>>("Horn Boost Effect", "Adds a Pog Turbo Effect", &features::hornboosteffect, BoolDisplay::OnOff);
				}
				sub->AddOption<NumberOption<std::int32_t>>("Vehicle Speed", nullptr, &features::vehiclespeed, 0, 500, 10, 3, true, "< ", " >", [&] {});
				sub->AddOption<RegularOption>("Set Speed", "", [&]
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
				sub->AddOption<BoolOption<bool>>("Rainbow colour", "Makes a pog loop rgb fade", &features::rainbowcar, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Colour spam", "Wait i hope you dont have epilepsy...", &features::flashrainbow, BoolDisplay::OnOff);

				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~r~R~s~]", nullptr, &features::VEHr, 0, 255, 10, 3, true, "< ", " >", [&] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~g~G~s~]", nullptr, &features::VEHg, 0, 255, 10, 3, true, "< ", " >", [&] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~b~B~s~]", nullptr, &features::VEHb, 0, 255, 10, 3, true, "< ", " >", [&] {});
				sub->AddOption<RegularOption>("Set Vehicle Primary RGB", "Random Paint Vehicle", [&]
					{
						static const int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, features::VEHr, features::VEHg, features::VEHb);
					});
				
				sub->AddOption<RegularOption>("Set Vehicle Secondary RGB", "Random Paint Vehicle", [&]
					{
						static const int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(VehID, features::VEHr, features::VEHg, features::VEHb);
					});
				sub->AddOption<RegularOption>("Random Paint", "Random Paint Vehicle", [&]
					{
						static const int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
						VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(VehID, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle", SubmenuVehicle, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Vehicle Spawner", nullptr, SubmenuVehicleSpawner);
				sub->AddOption<SubOption>("Vehicle Movement", nullptr, SubmenuVehicleMovement);
				sub->AddOption<SubOption>("Vehicle Acrobatics", nullptr, SubmenuVehicleAcrobatics);
				sub->AddOption<SubOption>("Vehicle Colors", nullptr, SubmenuVehicleColors);
				sub->AddOption<BoolOption<bool>>("Vehicle Godmode", nullptr, &features::vehgodmode, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Max Tuning", "Tune the Vehicle To Maximum", [&]
					{
						const int Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(features::g_selected_player), 0);;
						features::maxvehicle(Vehicle);
					});
				sub->AddOption<RegularOption>("Add Ramp", "Tune the Vehicle To Maximum", [&]
					{
						features::AddRamp();
					});
				sub->AddOption<RegularOption>("Repair Vehicle", "Repair Damages To Currnet Vehicle", [&]{features::repairVehicle();});
				sub->AddOption<BoolOption<bool>>("Repair Vehicle Loop", "Repair Vehicle Automatically When Demaged", &features::fixloop, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Seatbelt", "You Cant Fall Of Vehicle", &features::seatbelt, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Invisible Vehicle", "You cant see your car", &features::invis_car, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Duplicate Current Vehicle", "Spawn Another One", [&]
					{
						features::duplicatecar();
					});
				//sub->AddOption<RegularOption>("Set License Plate", "Set Custom Text", [&]
				//	{
				//		CharKeyboard("Text", 21, "nig");
				//		const char*  result = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
				//		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false), result);
				//	});
				sub->AddOption<RegularOption>("Clean Vehicle", "Clean Currnet Vehicle", [&]
					{features::cleanVehicle();	});
				sub->AddOption<BoolOption<bool>>("Clean Vehicle Loop", "Cleans Vehicle Automatically When Dirty", &features::cleanloop, BoolDisplay::OnOff);

				sub->AddOption<RegularOption>("Stop Vehicle", "Set Vehicle Stationary", [&]
					{
						Vehicle Veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Veh);
						if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Veh))
						{
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(Veh, 0);
						}
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Spawner", SubmenuVehicleSpawner, [&](RegularSubmenu* sub)
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

		g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Vehicle Acrobatics"), SubmenuVehicleAcrobatics, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>(xorstr_("Forward Flip"), xorstr_("Forward Flip"), [&] {	features::acrobatics(Frontflip);	});

				sub->AddOption<RegularOption>(xorstr_("Forward Flip"), xorstr_("Forward Flip"), [&] {	features::acrobatics(Backflip);	});

				sub->AddOption<RegularOption>(xorstr_("Left Flip"), xorstr_("Forward Flip"), [&]{	features::acrobatics(LeftFlip);	});

				sub->AddOption<RegularOption>(xorstr_("Right Flip"), xorstr_("Forward Flip"), [&]{	features::acrobatics(RightFlip);	});
				sub->AddOption<RegularOption>(xorstr_("Jump Up"), xorstr_("Jump Up"), [&]{	features::acrobatics(JumpUp);	});
				sub->AddOption<RegularOption>(xorstr_("Launch Up"), xorstr_("Jump Up"), [&]{	features::acrobatics(LaunchUp);	});
				
				sub->AddOption<NumberOption<float>>(xorstr_("Power"), nullptr, &features::z, -10, 20, 1, 3, true, "< ", " >", [&] {});
				sub->AddOption<NumberOption<float>>(xorstr_("Offset X"), nullptr, &features::offX, -10, 20, 1, 3, true, "< ", " >", [&] {});
				sub->AddOption<NumberOption<float>>(xorstr_("Offset Y "), nullptr, &features::offY, -10, 20, 1, 3, true, "< ", " >", [&] {});
				sub->AddOption<NumberOption<float>>(xorstr_("Offset Z"), nullptr, &features::offZ, -10, 20, 1, 3, true, "< ", " >", [&] {});
				sub->AddOption<RegularOption>(xorstr_("Custom acrobatics"), xorstr_("Perform"), [&]
					{
						features::perform();
					});

			
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Spawner Settings", SubmenuVehSpawnerSettings, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Spawn Invincible", "Vehicle Will Be Spawned With Godmode", &features::vehicle_godmode, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn Inside", "You Will Be Spawned Inside Vehicle", &features::in_vehicle, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn Max Tuned", "Vehicle Will HAve Max Tuning", &features::full_stats, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn With Blip", "You Will See The Vehicle On Map", &features::vehicle_blip, BoolDisplay::OnOff);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sports", SubmenuVehSpawnerSports, [](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Sports1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Muscle", SubmenuVehSpawnerMuscle, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Muscle1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Super", SubmenuVehSpawnerSuper, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Super1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sport Classics", SubmenuVehSpawnerSportClassics, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::SportsClassics1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sedans", SubmenuVehSpawnerSedans, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Sedans1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("SUVs", SubmenuVehSpawnerSUVs, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::SUVs1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Compacts", SubmenuVehSpawnerCompacts, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Compacts1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Coupes", SubmenuVehSpawnerCoupes, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Coupes1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Motorcycles", SubmenuVehSpawnerMotorcycles, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Motorcycles1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Off-Road", SubmenuVehSpawnerOffRoad, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::OffRoad1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Cycles", SubmenuVehSpawnerCycles, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Cycles1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Vans", SubmenuVehSpawnerVans, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Vans1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Utility", SubmenuVehSpawnerUltility, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Utility1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Industrial", SubmenuVehSpawnerIndustrial, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Industrial1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sevice", SubmenuVehSpawnerService, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Service1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Commercial", SubmenuVehSpawnerCommercial, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Commercial1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Emergency", SubmenuVehSpawnerEmergency, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Emergency1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Military", SubmenuVehSpawnerMilitary, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Military1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Boats", SubmenuVehSpawnerBoats, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Boats1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Planes", SubmenuVehSpawnerPlanes, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Planes1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Helicopters", SubmenuVehSpawnerHelicopters, [&](RegularSubmenu* sub)
			{
				for (auto& car : Lists::Helicopters1) {
					sub->AddOption<RegularOption>(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(car))), "Spawn this car", [car]
						{
							features::spawn_veh(rage::joaat(car));
						});
				}
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Online", OnlineSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Players", "All players in session", SubmenuPlayerList);
				sub->AddOption<SubOption>("All Players", "You can mess with all of them in once", SessionAllPlayersSubmenu);
				sub->AddOption<SubOption>("Session settings", "Modify The Session", SessionSettingsSubmenu);
				sub->AddOption<SubOption>("Session Joiner", "Join Session You Like", SessionJoinerSubmenu);
				
				
					
				//
				//sub->AddOption<RegularOption>("RID test", nullptr, [&]
				//	{
				//		static bool done = false;

				//		g_thread_pool->push([&] //thread pool for not making fps lag
				//			{
				//				features::rid = get_rid_from_name("Yodo2222");
				//				done = true;
				//			});

				//		if (done)
				//			LOG(INFO) << features::rid;

				//		// You can use keyboard get input func for getting target name and then display it on option text
				//	});
			});
			g_UiManager->AddSubmenu<RegularSubmenu>("Online", SessionJoinerSubmenu, [&](RegularSubmenu* sub)
				{
					sub->AddOption<RegularOption>("Join Online", nullptr, [&] {features::change_session(features::GTAO); });
					sub->AddOption<RegularOption>("Find Public Session", nullptr, [&] {features::change_session(features::FindPublicSession); });
					sub->AddOption<RegularOption>("Start Publuc Session", nullptr, [&] {features::change_session(features::StartNewPublicSession); });
					sub->AddOption<RegularOption>("Start Crew Closed Session", nullptr, [&] {features::change_session(features::ClosedCrewSession); });
					sub->AddOption<RegularOption>("Start Crew Session", nullptr, [&] {features::change_session(features::JoinCrewSession); });
					sub->AddOption<RegularOption>("Closed Friends Session", nullptr, [&] {features::change_session(features::ClosedFriendSession); });
					sub->AddOption<RegularOption>("Join Friends", nullptr, [&] {features::change_session(features::FindFriendsSession); });
					sub->AddOption<RegularOption>("Start Solo Session", nullptr, [&] {features::change_session(features::StartSoloSession); });
					sub->AddOption<RegularOption>("Start Invite Only Session", nullptr, [&] {features::change_session(features::InviteOnlySession); });
					sub->AddOption<RegularOption>("Join Crew", nullptr, [&] {features::change_session(features::JoinCrewSession); });
					sub->AddOption<RegularOption>("Join SCTV", nullptr, [&] {features::change_session(features::JoinSCTV); });
				});
		g_UiManager->AddSubmenu<RegularSubmenu>("All Players", SessionAllPlayersSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Teleport", "All players in session", AllPlayerTeleport);
				sub->AddOption<SubOption>("Abuse", "You can mess with all of them in once", AllPlayerAbuse);
				sub->AddOption<SubOption>("Vehicle", "Modify The Session", AllPlayerVehicle);
				sub->AddOption<SubOption>("Trolling", "You can mess with all of them in once", AllPlayerTrolling);
				sub->AddOption<SubOption>("Friendly", "Modify The Session", AllPlayerFriendly);
				sub->AddOption<SubOption>("ESP", "Know where other players are", AllPlayersESP);

				});

		g_UiManager->AddSubmenu<RegularSubmenu>("ESP All Players", AllPlayersESP, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Line ESP", "You will get guided where other players are", &features::esp_line_all, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Box ESP", "You will get guided where other players are", &features::esp_box_all, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Name ESP", "You will get guided where other players are", &features::esp_name_all, BoolDisplay::OnOff);

			});
				g_UiManager->AddSubmenu<RegularSubmenu>("Player Teleports", AllPlayerTeleport, [&](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

						sub->AddOption<RegularOption>("Teleport To Player", "tp to Player", [&]
							{
								Entity handle;
								PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
								ENTITY::SET_ENTITY_COORDS(handle, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).z, false, false, false, false);
							});
						sub->AddOption<RegularOption>("Teleport To Player's Vehicle", "tp to Players vehicle", [&]
							{
								if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(NULL), true))
								{
									PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(features::g_selected_player)), VEHICLE::IS_VEHICLE_SEAT_FREE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(NULL)), -1, true));
								}
							});
					});

				g_UiManager->AddSubmenu<RegularSubmenu>("Abuse", AllPlayerAbuse, [&](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

						if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == PLAYER::PLAYER_PED_ID())
						{
							sub->AddOption<RegularOption>("Host Kick", "Kick From Session", [&]
								{
									NETWORK::NETWORK_SESSION_KICK_PLAYER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
									int hostkick[69] = { 1885259, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) };
									SCRIPT::TRIGGER_SCRIPT_EVENT(1, hostkick, 4, 1 << features::g_selected_player);
									//1885259 host kick global
								});
						}
						sub->AddOption<RegularOption>("Kick", "Kick ", [&]
							{
								for (int i = 0; i < 32; i++)
								{
									features::kick(i);
								}

							});
						sub->AddOption<RegularOption>("Nig Nog Crash", "Nigs them out from game", [&]
							{

								for (int i = 0; i < 32; i++)
								{
									features::crash(i);
								}

							});
						sub->AddOption<RegularOption>("Test Crash", "Test Crash From Session", [&]
							{

								for (int i = 0; i < 32; i++)
								{
									features::infloading(i);
								}

							});
						sub->AddOption<RegularOption>("Script Kick", "Kick From Session", [&]
							{
								std::uint64_t args_1[] = { -145306724, 1, 0, 0 };

								g_pointers->m_TriggerScriptEvent(1, args_1, 4, NULL);

							});
					});
				g_UiManager->AddSubmenu<RegularSubmenu>("Players Vehicle Options", AllPlayerVehicle, [&](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);
						sub->AddOption<BoolOption<bool>>("Explode Oppressors", "This will explode anyone who is on oppressor mk2", &features::kick_from_mk2, BoolDisplay::OnOff);


					});

				g_UiManager->AddSubmenu<RegularSubmenu>("Trolling", AllPlayerTrolling, [&](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

						
						sub->AddOption<RegularOption>("Clone Player", "Clones Player", [&]
							{
								PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1, 1, 1);
							});
						sub->AddOption<RegularOption>("Attach To Player", "Attaches You To Player", [&]
							{
								if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) == PLAYER::PLAYER_PED_ID())
								{

								}
								else
								{
									ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, false, true);
								}
							});
						sub->AddOption<RegularOption>("Detach Off Player", "Detach You From Player", [&]
							{
								ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), true, true);
							});
						sub->AddOption<RegularOption>("Explode Player", "Blow Up Player", [&]
							{
								Vector3 targetCords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
								FIRE::ADD_EXPLOSION(targetCords.x, targetCords.y, targetCords.z, 29, 9999.0f, true, false, 0.0f, false);
							});
						sub->AddOption<RegularOption>("Give Him Wanted Level", "Give The Player Wanted Level", [&]
							{
								PLAYER::SET_PLAYER_WANTED_LEVEL(features::g_selected_player, 5, false);
								PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
							});
						sub->AddOption<BoolOption<bool>>("Fuck Their Camera", "Just a Little Trolling", &features::fucktheircam, BoolDisplay::OnOff);
						sub->AddOption<RegularOption>("Airstrike Player", "Blow Up Player With Airstrike", [&]
							{
								Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
								Ped playerPed = PLAYER::PLAYER_PED_ID();
								Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
								Hash airStrike = rage::joaat("WEAPON_AIRSTRIKE_ROCKET");
								WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
								MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 50.f, coords.x, coords.y, coords.z, 250, 1, airStrike, playerPed, 1, 0, -1.0);

							});
						sub->AddOption<RegularOption>("Cage Player", "Trap In cage", [&]
							{
								for (int i = 0; i < 32; i++)
								{
									OBJECT::CREATE_OBJECT(959275690, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false).z - 1.f, true, false, false);
								}
							});
						sub->AddOption<RegularOption>("Tube Player", "Trap In Tube", [&]
							{
								Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0);
								Object obj = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("stt_prop_stunt_tube_fn_01"), remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, true);
								ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 90.0f, 0.0f, 0, true);
							});
						sub->AddOption<RegularOption>("Tree Player", "Trap In Tree", [&]
							{
								Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
								Object obj1 = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("prop_xmas_tree_int"), pos.x, pos.y, pos.z, 1, 1, 1);
							});

					});

				g_UiManager->AddSubmenu<RegularSubmenu>("Friendly", AllPlayerFriendly, [&](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

					});

			
		g_UiManager->AddSubmenu<RegularSubmenu>("Session Settings", SessionSettingsSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Notify For Money Drop", "You Will Get Notified When Someone Drops Money", &features::moneynotify, BoolDisplay::OnOff);
				sub->AddOption<SubOption>("Admin Detection", "", RockstarAdminSubmenu);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Admin Detection", RockstarAdminSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Rockstar Admin Detection", "You Know What It Is", &features::rockstaradmin, BoolDisplay::OnOff);
				if (features::rockstaradmin)
				{

				
				sub->AddOption<BoolOption<bool>>("Notify On Detection", "You Will Get Notified When There Is R* Admin In Your Session", &features::notifyadmin, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Leave On Detection", "You Will Automatically Leave When There Is R* Admin In Your Session", &features::leaveondetect, BoolDisplay::OnOff);
				if (!features::leaveondetect)
				{
					sub->AddOption<BoolOption<bool>>("Exit Game", "Your Game Will Turn Off When There Is A Rockstar Admin", &features::crashgame, BoolDisplay::OnOff);

				}

				}
				
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Weapon Menu", Weaponz, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Give All Weapons", "Gives you all weapons", [&]{features::GiveAllWeapons();});
				sub->AddOption<RegularOption>("Max Ammo", "Max Ammo", [&]{features::MaxAmmo();});
				sub->AddOption<RegularOption>("Remove Weapons", "Remove Weapons", [&]
					{
						features::RemoveAllWeapons();
					});
				//sub->AddOption<BoolOption<bool>>("Always Crosshair", "Bruh Read The Title", &features::croshair, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Infinite ammo", "Your gun will never be empty", &features::infiniteammo, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Delete Gun", "Your gun will never be empty", &features::deletegun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Aimbot", "Are you a noob?", &features::aimbot, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Explosive Gun", "Your gun will never be empty", &features::exploammo, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Airstrike Gun", "A Plane Will Strike Where You Shoot", &features::airstrikegun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Teleport Gun", "You Will Teleport Where You Shoot", &features::teleportgun, BoolDisplay::OnOff);
				
				sub->AddOption<BoolOption<bool>>("Drive It Gun", "You Will Enter The Vehicle You Shoot At", &features::driveitgun, BoolDisplay::OnOff);
				
				sub->AddOption<BoolOption<bool>>("Shoot Peds ", "YOu will shoot pedestrians", &features::shootpeds, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Dead eye", "Matrix brooo", &features::deadeye, BoolDisplay::OnOff);

				
				

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("World_Options", WorldOptions, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Time", "Adjust the World Time", TimeSubmenu);
				sub->AddOption<SubOption>("Weather", "Adjust the World Time", WeatherSubmenu);
				//sub->AddOption<SubOption>("Water", "Mess With Water", WaterSubmenu);
				sub->AddOption<SubOption>("Clear Area", "Select what to clear from you", ClearSubmenu);
				sub->AddOption<NumberOption<std::int32_t>>("World Gravity", nullptr, &features::worldgravity, 0, 3, 1, 3, true, "< ", " >", [&] {});
				sub->AddOption<RegularOption>("Set Gravity", "", [&]
					{
						MISC::SET_GRAVITY_LEVEL(features::worldgravity);
					});
				sub->AddOption<NumberOption<float>>("Rain Intensity", nullptr, &features::rainlevelint, 0, 1000, 10, 3, true, "< ", " >", [&] {});
				sub->AddOption<RegularOption>("Set Intensity", "", [&]
					{
						MISC::_SET_RAIN_FX_INTENSITY(features::rainlevelint);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Time", TimeSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Stop Time", "Freezes Time In the World", &features::stoptime, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<int>>("Hours", "Might Not Work In Online!", &features::HoursTime, 0, 23, 1, 1, true, "< ", " >", [&]
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, 0, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, 0, 0);
					});
				sub->AddOption<NumberOption<int>>("Minutes", "Might Not Work In Online!", &features::MinutesTime, 0, 59, 1, 1, true, "< ", " >", [&]
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
					});
				sub->AddOption<RegularOption>("Add 1 Hour", "Adds 1 Hour to Current World Time", [&]
					{
						CLOCK::ADD_TO_CLOCK_TIME(1, 0, 0);
					});
				sub->AddOption<RegularOption>("Reduce 1 Hour", "Removes 1 Hour of Current World Time", [&]
					{
						CLOCK::ADD_TO_CLOCK_TIME(-1, 0, 0);
					});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Weather", WeatherSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Extra Sunny", "Set Weather Extra Sunny", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("EXTRASUNNY");
					});
				sub->AddOption<RegularOption>("Clean", "Set Weather Clean", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("CLEAR");
					});
				sub->AddOption<RegularOption>("Clearing", "Set Weather Clearing", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("CLEARING");
					});
				sub->AddOption<RegularOption>("Cloudy", "Set Weather Cloudy", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("CLOUDS");
					});
				sub->AddOption<RegularOption>("Smog", "Set Weather Smog", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("SMOG");
					});
				sub->AddOption<RegularOption>("Foggy", "Set Weather Foggy", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("FOGGY");
					});
				sub->AddOption<RegularOption>("Overcast", "Set Weather Overcast", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("OVERCAST");
					});
				sub->AddOption<RegularOption>("Rain", "Set Weather Rain", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("RAIN");
					});
				sub->AddOption<RegularOption>("Thunder", "Set Weather Thunder", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("THUNDER");
					});
				sub->AddOption<RegularOption>("Neutral", "Set Weather Neutral", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("NEUTRAL");
					});
				sub->AddOption<RegularOption>("Snow", "Set Weather Snow", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("SNOW");
					});
				sub->AddOption<RegularOption>("Blizzard", "Set Weather Blizzard", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("BLIZZARD");
					});
				sub->AddOption<RegularOption>("Light Snow", "Set Weather Light Snow", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("SNOWLIGHT");
					});
				sub->AddOption<RegularOption>("Xmas", "Set Weather Xmas", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("XMAS");
					});
				sub->AddOption<RegularOption>("Halloween", "Set Weather Halloween", [&]
					{
						MISC::SET_OVERRIDE_WEATHER("HALLOWEEN");
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Water", WaterSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<NumberOption<int>>("Hours", "Might Not Work In Online!", &features::HoursTime, 0, 23, 1, 1, true, "< ", " >", [&]
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, 0, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, 0, 0);
					});
				sub->AddOption<NumberOption<int>>("Minutes", "Might Not Work In Online!", &features::MinutesTime, 0, 59, 1, 1, true, "< ", " >", [&]
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, features::MinutesTime, 0);
					});
				sub->AddOption<RegularOption>("Add 1 Hour", "Adds 1 Hour to Current World Time", [&]
					{
						CLOCK::ADD_TO_CLOCK_TIME(1, 0, 0);
					});
				sub->AddOption<RegularOption>("Reduce 1 Hour", "Removes 1 Hour of Current World Time", [&]
					{
						CLOCK::ADD_TO_CLOCK_TIME(-1, 0, 0);
					});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Clear Area", ClearSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<NumberOption<int>>("Radius", "How far should objects be cleared?", &features::clearradius, 1, 500, 10, 1, true, "< ", " >", [&]{		NULL;	});
				sub->AddOption<RegularOption>("Clear From Everything", "Deletes All Objects", [&]
					{	features::Clearall();	});
				sub->AddOption<RegularOption>("Delete Vehicles", "Deletes All Vehicles In Area", [&]
					{	features::Clearcars();	});
				sub->AddOption<RegularOption>("Delete Peds", "Deletes All Peds In Area", [&]
					{	features::Clearpeds();	});
				sub->AddOption<RegularOption>("Delete Cops", "Deletes All Cops In Area", [&]
					{	features::Clearcops();	});

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Teleport Menu", teleports, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Teleport To Waypoint ", "Teleport to Waypoint", [&]{
						features::teleport_to_waypoint();
					});
				sub->AddOption<RegularOption>("Teleport To Objective ", "Teleport to Objective", [&] { {features::teleport_to_objective(); }});
			
				sub->AddOption<SubOption>("Airport's", nullptr, air);
				sub->AddOption<SubOption>("IPL's", nullptr, ipls);
				sub->AddOption<SubOption>("Landmarks", nullptr, landmarks);
				sub->AddOption<SubOption>("Garages", nullptr, gar);
				sub->AddOption<SubOption>("Story Safe Houses", nullptr, safehouse);
				sub->AddOption<SubOption>("Shops", nullptr, shops);

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Garage", gar, [&](RegularSubmenu* sub)
			{
				for (auto location : teleportsGarages)
				{
					sub->AddOption<RegularOption>(location.name, "Teleports you in there", [&]
						{
							features::teleport(location.x, location.y, location.z);
						});

				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Safe Houses", safehouse, [&](RegularSubmenu* sub)
			{
				for (auto& location : teleportsSafehouses)
				{
					sub->AddOption<RegularOption>(location.name, "Teleports you in there", [&]
						{
							features::teleport(location.x, location.y, location.z);
						});

				}
				
			
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Shops", shops, [&](RegularSubmenu* sub)
			{
				for (auto location : teleportsShops)
				{
					sub->AddOption<RegularOption>(location.name, "Teleports you in there", [&]
						{
							features::teleport(location.x, location.y, location.z);
						});
				}
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Airports", air, [&](RegularSubmenu* sub)
			{
				for (auto& teleport : teleportsAirports)
				{
					sub->AddOption<RegularOption>(teleport.name, "Teleports you in there", [&]
						{
							features::teleport(teleport.x, teleport.y, teleport.z);
						});
				}						
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("IPls", ipls, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("North Yankton ", "Prologue Vibes", [&]
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
				sub->AddOption<RegularOption>("Porn Yacht ", "woo", [&]
					{
						STREAMING::REQUEST_IPL("smboat");
						Vector3 Coords;
						Coords.x = -2045.8f; Coords.y = -1031.2f; Coords.z = 11.9f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -2045.8f, -1031.2f, 11.9f);

					});
				sub->AddOption<RegularOption>("Life Invader Office", "Teleport To Life Invader Office", [&]
					{
						STREAMING::REQUEST_IPL("facelobby");
						STREAMING::REQUEST_IPL("facelobbyfake");
						Vector3 Coords;
						Coords.x = -1047.9f; Coords.y = -233.0f; Coords.z = 39.0f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), -1047.9f, -233.0f, 39.0f);

					});
				sub->AddOption<RegularOption>("Cargoship", "Teleport To Cargoship", [&]
					{
						STREAMING::REQUEST_IPL("cargoship");
						Vector3 Coords;
						Coords.x = -162.8918f; Coords.y = -2365.769f; Coords.z = 9.3192f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);

					});
				sub->AddOption<RegularOption>("Oneil Farm", "Teleport To Oneil Farm", [&]
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
				sub->AddOption<RegularOption>("Hospital ", "Teleport To hotspital", [&]
					{
						STREAMING::REQUEST_IPL("RC12B_HospitalInterior");
						STREAMING::REQUEST_IPL("RC12B_Destroyed");
						Vector3 Coords;
						Coords.x = 356.8f; Coords.y = -590.1f; Coords.z = 43.3f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
						;
					});
				sub->AddOption<RegularOption>("Jewelry Store", "Teleport To Jewelry Store", [&]
					{
						STREAMING::REQUEST_IPL("jewel2fake");
						STREAMING::REQUEST_IPL("post_hiest_unload");
						STREAMING::REQUEST_IPL("bh1_16_refurb");
						Vector3 Coords;
						Coords.x = -630.4f; Coords.y = -236.7f; Coords.z = 40.0f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);

					});
				sub->AddOption<RegularOption>("Morgue", "Teleport To Morgue", [&]
					{
						STREAMING::REQUEST_IPL("Coroner_Int_on");
						Vector3 Coords;
						Coords.x = 244.9f; Coords.y = -1374.7f; Coords.z = 39.5f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Mandrazo Ranch", "Teleport To Martins Ranch", [&]
					{
						Vector3 Coords;
						Coords.x = 1371.55f; Coords.y = 1147.55f; Coords.z = 114.775f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
				sub->AddOption<RegularOption>("Prison", "Teleport To Prison", [&]
					{
						Vector3 Coords;
						Coords.x = 1860.29f; Coords.y = 2607.89f; Coords.z = 47.1749f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
			});


		g_UiManager->AddSubmenu<RegularSubmenu>("Landmarks", landmarks, [&](RegularSubmenu* sub)
			{

				for (auto& landmark : teleportsLandmarks)
				{
					sub->AddOption<RegularOption>(landmark.name, "Teleports you in there", [&]
						{
							features::teleport(landmark.x, landmark.y, landmark.z);
						});
				}
			
				
				sub->AddOption<RegularOption>("The Setup", "Oh Its the setup its the setup...", [&]
					{
						CLOCK::SET_CLOCK_TIME(19, 0, 0);
						Vector3 Coords;
						Coords.x = -1577.67f; Coords.y = 5159.83f; Coords.z = 21.9807f;
						PED::SET_PED_COORDS_KEEP_VEHICLE(PLAYER::PLAYER_PED_ID(), Coords.x, Coords.y, Coords.z);
					});
			});


		g_UiManager->AddSubmenu<RegularSubmenu>("Players", SubmenuPlayerList, [&](RegularSubmenu* sub)
			{
				
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				for (std::uint32_t i = 0; i < 32; ++i)
				{
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
					{
						if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == i)
						{
							char Buffer[255]; sprintf(Buffer, "%s   ~p~[SESSION HOST]", PLAYER::GET_PLAYER_NAME(i));  /////// HOST 
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [&]
								{
									features::g_selected_player = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == PLAYER::PLAYER_PED_ID())
						{
							char Buffer[255]; sprintf(Buffer, "%s   ~b~[SELF]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [&]
								{
									features::g_selected_player = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == NETWORK::NETWORK_IS_FRIEND_IN_MULTIPLAYER(PLAYER::GET_PLAYER_NAME(i))) {
							char Buffer[255]; sprintf(Buffer, "%s   ~g~[FRIEND]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [&]
								{
									features::g_selected_player = i;
								});
						}
						else if (g_player_list.is_modder) {
							char Buffer[255]; sprintf(Buffer, "%s   ~r~[MODDER]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [&]
								{
									features::g_selected_player = i;
								});
						}
						// 
						else
						{
							sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [&]
								{
									features::g_selected_player = i;
								});
						}
					}
				}
			});

		g_UiManager->AddSubmenu<PlayerSubmenu>(&features::g_selected_player, SubmenuSelectedPlayer, [&](PlayerSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);
				
				

				sub->AddOption<BoolOption<bool>>("Spectate", nullptr, &features::spectateplayer, BoolDisplay::OnOff);
				sub->AddOption<SubOption>("Teleport Options", nullptr, SelectedPlayerTeleport);
				sub->AddOption<SubOption>("Vehicle Options", nullptr, SelectedPlayerVehicle);
				sub->AddOption<SubOption>("Abuse", nullptr, SelectedPlayerAbuse);
				sub->AddOption<SubOption>("Trolling Options", nullptr, SelectedPlayerTrolling);
				sub->AddOption<SubOption>("Friendly Options", nullptr, SelectedPlayerFriendly);
				sub->AddOption<SubOption>("Teleport Options", nullptr, SelectedPlayerESP);
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Player ESP", SelectedPlayerESP, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Line ESP", nullptr, &features::line_esp, BoolDisplay::OnOff);

			});


		g_UiManager->AddSubmenu<RegularSubmenu>("Player Teleports", SelectedPlayerTeleport, [&](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				sub->AddOption<RegularOption>("Teleport To Player", "tp to Player", [&]
					{
						Entity handle;
						PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
						ENTITY::SET_ENTITY_COORDS(handle, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).z, false, false, false, false);
					});
				sub->AddOption<RegularOption>("Teleport To Player's Vehicle", "tp to Players vehicle", [&]
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(NULL), true))
						{
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(features::g_selected_player)), VEHICLE::IS_VEHICLE_SEAT_FREE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(NULL)), -1, true));
						}
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Abuse", SelectedPlayerAbuse, [&](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == PLAYER::PLAYER_PED_ID())
				{
					sub->AddOption<RegularOption>("Host Kick", "Kick From Session", [&]
						{
							NETWORK::NETWORK_SESSION_KICK_PLAYER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
							int hostkick[69] = { 1885259, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) };
							SCRIPT::TRIGGER_SCRIPT_EVENT(1, hostkick, 4, 1 << features::g_selected_player);
							//1885259 host kick global
						});
				}
				sub->AddOption<RegularOption>("Kick", "Kick ", [&]
					{
						features::kick(features::g_selected_player);
					});
				sub->AddOption<RegularOption>("Nig Nog Crash", "Crash From Game", [&]
					{

						features::crash(features::g_selected_player);

					});
				sub->AddOption<RegularOption>("Infinite Loading", "He Will be In Loadingscreen Forever", [&]
					{

						features::infloading(features::g_selected_player);

					});
				sub->AddOption<RegularOption>("Transaction error", "They will be in infinite transaction", [&]
					{

						features::transactionerror(features::g_selected_player);

					});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Players Vehicle Options", SelectedPlayerVehicle, [&](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);
				sub->AddOption<RegularOption>("Boost Player sCar", "Knock him off", [&]
					{
						features::boostPlayer();
					});

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Trolling", SelectedPlayerTrolling, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Ragdoll Player", "Knock him off", [&]
					{
						features::Ragdoll_player();
					});
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				sub->AddOption<RegularOption>("Clone Player", "Clones Player", [&]
					{
						PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1, 1, 1);
					});
				sub->AddOption<RegularOption>("Attach To Player", "Attaches You To Player", [&]
					{
						if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) == PLAYER::PLAYER_PED_ID())
						{

						}
						else
						{
							ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, false, true);
						}
					});
				sub->AddOption<RegularOption>("Detach Off Player", "Detach You From Player", [&]
					{
						ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), true, true);
					});
				sub->AddOption<RegularOption>("Explode Player", "Blow Up Player", [&]
					{
						Vector3 targetCords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
						FIRE::ADD_EXPLOSION(targetCords.x, targetCords.y, targetCords.z, 29, 9999.0f, true, false, 0.0f, false);
					});
				sub->AddOption<RegularOption>("Give Him Wanted Level", "Give The Player Wanted Level", [&]
					{
						PLAYER::SET_PLAYER_WANTED_LEVEL(features::g_selected_player, 5, false);
						PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
					});
				sub->AddOption<BoolOption<bool>>("Fuck Their Camera", "Just a Little Trolling", &features::fucktheircam, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Freeze Player", "They wont be able to move", &features::freeze_player, BoolDisplay::OnOff);

				sub->AddOption<RegularOption>("Airstrike Player", "Blow Up Player With Airstrike", [&]
					{
						Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
						Ped playerPed = PLAYER::PLAYER_PED_ID();
						Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
						Hash airStrike = rage::joaat("WEAPON_AIRSTRIKE_ROCKET");
						WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 50.f, coords.x, coords.y, coords.z, 250, 1, airStrike, playerPed, 1, 0, -1.0);

					});
				sub->AddOption<RegularOption>("Cage Player", "Trap In cage", [&]
					{
						OBJECT::CREATE_OBJECT(959275690, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).z - 1.f, true, false, false);
					});
				sub->AddOption<RegularOption>("Tube Player", "Trap In Tube", [&]
					{
						Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0);
						Object obj = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("stt_prop_stunt_tube_fn_01"), remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, true);
						ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 90.0f, 0.0f, 0, true);
					});
				sub->AddOption<RegularOption>("Tree Player", "Trap In Tree", [&]
					{
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
						Object obj1 = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("prop_xmas_tree_int"), pos.x, pos.y, pos.z, 1, 1, 1);
					});

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Friendly", SelectedPlayerFriendly, [&](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

			});





		//		sub->AddOption<RegularOption>("Set Waypoint", "tp to Players vehicle", [&]
		//			{
		//				std::int32_t sp = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
		//				if (ENTITY::DOES_ENTITY_EXIST(sp)) {
		//					Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(sp, 0.0f, 0.0f, 0.0f);
		//					HUD::SET_NEW_WAYPOINT(coords.x, coords.y);
		//				}
		//			});

		//		sub->AddOption<RegularOption>("Copy Outfit", "Steal Players Outfit", [&]
		//			{
		//				for (int i = 0; i < 12; i++)
		//				{
		//					PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), i), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), i), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL), i));
		//					for (int i = 0; i < 2; i++) {
		//					}
		//				}
		//			});

		//	});

		g_UiManager->AddSubmenu<RegularSubmenu>("Recovery", recovery, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Money", nullptr, MoneySubmenu);
				sub->AddOption<SubOption>("Level", nullptr, LevelSubmenu);
				sub->AddOption<SubOption>("Heist Helpers", nullptr, HeistSubmenu);
				sub->AddOption<RegularOption>("Unlock Everything", "Make sure to be atleast level 250", [&] {features::unlock_everything; AUDIO::PLAY_SOUND_FRONTEND(-1, xorstr_("CHALLENGE_UNLOCKED"), xorstr_("HUD_AWARDS"), 1); });
				sub->AddOption<RegularOption>("Unlock Achievements", "Unlocks all achievements", [&] {features::unlock_achievements; AUDIO::PLAY_SOUND_FRONTEND(-1, xorstr_("CHALLENGE_UNLOCKED"), xorstr_("HUD_AWARDS"), 1); });
				sub->AddOption<RegularOption>("Set 2013 creation date", "Your account will appear as 8 years old", [&] {features::max_creation_date(); });

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Level", LevelSubmenu, [&](RegularSubmenu* sub)
			{
			
				sub->AddOption<NumberOption<int>>("Level", nullptr, &features::level, 0, 8000, 1, 1);
				sub->AddOption<RegularOption>("Set Level", "Adds level you selected", [&] {features::set_rank(features::level); AUDIO::PLAY_SOUND_FRONTEND(-1, xorstr_("CHALLENGE_UNLOCKED"), xorstr_("HUD_AWARDS"), 1); });
				sub->AddOption<RegularOption>("Set Crew Level", "Adds level you selected", [&] {features::set_crew_rank(features::level); AUDIO::PLAY_SOUND_FRONTEND(-1, xorstr_("CHALLENGE_UNLOCKED"), xorstr_("HUD_AWARDS"), 1); });

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Money", MoneySubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("500k  Startup", "Only use to get enough on bunker", [&] {features::startupmoney(); });
				/*sub->AddOption<NumberOption<int>>("Amount", nullptr, &features::bunker_money, 0, 2500000, 100000, 1);
				sub->AddOption<RegularOption>("Add Money", "Adds selected amount", [&] {features::addMoney(); });*/
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Heist Helpers", HeistSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Cayo Perico - Skip preps", "skips the heist preps", [&] {features::cayoSkipPreps(); });
				sub->AddOption<RegularOption>("Cayo Perico - Enable Hard Mode", "Enabled heist hard mode", [&] {features::cayoHardMode(); });
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Miscellaneous", misc, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Object Spawner", nullptr, SubmenuSettingsObjectSpanwer);
				//sub->AddOption<SubOption>("Particles", nullptr, SubmenuSettingsParticles);
				//sub->AddOption<BoolOption<bool>>("Money Drop On Self [~r~RISKY]", "Vibe to the music everywhere!", &features::selfdrop, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Mobile Radio", "Vibe to the music everywhere!", &features::mobileradio, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Hide HUD", "Vibe to the music everywhere!", &features::hide_hud, BoolDisplay::OnOff);
				//sub->AddOption<BoolOption<bool>>("Show Coords", "Know Where you are", &features::coords_display, BoolDisplay::OnOff);

				sub->AddOption<BoolOption<bool>>("Disable Phone", "English Dave wont bother you", &features::nophone, BoolDisplay::OnOff);


				sub->AddOption<RegularOption>("Skip Cutscene", "Skips current cutscene", [&]
					{
						CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
					});

				
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Miscellaneous", misc, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Mobile Radio", "Vibe to the music everywhere!", &features::mobileradio, BoolDisplay::OnOff);
				//sub->AddOption<BoolOption<bool>>("Free Camera", "Vibe to the music everywhere!", &features::freecam, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Disable Phone", "English Dave wont bother you", &features::nophone, BoolDisplay::OnOff);

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Protections", ProtectionsSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Notify For Blocked Events", "", &features::g_received_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Crash Protection", "", &features::g_crash_protex, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Kick Protection", "", &features::g_kick_protex, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Log Net Events", "Logs the events in a txt file", &features::g_log_net_event_data, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Explosions", "Blocks Explosions from other players", &features::g_explosion_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Particles", "Blocks Particle FX Effects", &features::g_ptfx_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Freeze", "Players Cant Freeze You", &features::no_freeze_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Pickup", "This Should Block Pickups Spawning On You", &features::no_pickup_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Time Change", "Others Cant Mess With Time", &features::no_clock_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Sound Event", "Blocks Annoying Sounds From Players", &features::no_sound_effect, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Votekick", "", &features::no_votekick_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Weapons Events", "Players Cant Take Or Give You Weapons", &features::no_weapons_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("No Wanted Event", "players Cant Give You Wanted Level", &features::no_wanted_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Players Cant Mess With Weather", "", &features::no_weather_event, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Block Reports", "", &features::no_report_event, BoolDisplay::OnOff);


			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Language", SubmenuSettingsLanguage, [&](RegularSubmenu* sub)
			{
				/*std::filesystem::directory_iterator dirIt{ g_TranslationManager->GetTranslationDirectory() };
				for (auto&& dirEntry : dirIt)
				{

					auto path = dirEntry.path();
					if (path.has_filename())
					{
						sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [&]
							{
								g_TranslationManager->LoadTranslations(path.stem().u8string().c_str());
							});
					}

				}*/



			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Object Spawner", SubmenuSettingsObjectSpanwer, [&](RegularSubmenu* sub)
			{
				for (auto& obj : Lists::Objects1) {
					sub->AddOption<RegularOption>(obj, "Spawn this object", [obj]
						{
							features::spawn_obj(obj);
						});
				}

			});
	



		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [&](RegularSubmenu* sub)
			{
				sub->AddOption<RegularOption>("Switch UI", "Switches to click UI.", [&]
					{
						Auth a();
						g_list = false;
						g_gui.m_opened = true;
					});
					

				sub->AddOption<BoolOption<bool>>("Numpad Control", " 8 - Up | 2 - Down | 4 - Left | 6 - Right | 5 - Select |\n 0 - Back", &features::numpadcontrol, BoolDisplay::OnOff);

				sub->AddOption<RegularOption>("Unload", "Unload the menu.", [&]
					{
						g_running = false;
					});
				sub->AddOption<RegularOption>("Close Game", "Go to desktop", [&]
					{
						exit(0);
					});
				sub->AddOption<SubOption>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
				sub->AddOption<SubOption>("Links", nullptr, SUbmenuSettingsLinks);
				sub->AddOption<SubOption>("Info Windows", nullptr, SettingsInfoWindows);
				sub->AddOption<SubOption>("Options", nullptr, SubmenuSettingsOption);//SettingsInfoWindows
				sub->AddOption<SubOption>("Footer", nullptr, SubmenuSettingsFooter);
				sub->AddOption<SubOption>("Description", nullptr, SubmenuSettingsDescription);
				//sub->AddOption<SubOption>("Input", nullptr, SubmenuSettingsInput);
				//sub->AddOption<SubOption>("Language", nullptr, SubmenuSettingsLanguage);
				sub->AddOption<NumberOption<float>>("X Position", nullptr, &g_UiManager->m_PosX, 0.f, 1.f, 0.01f, 2);
				sub->AddOption<NumberOption<float>>("Y Position", nullptr, &g_UiManager->m_PosY, 0.f, 1.f, 0.01f, 2);
				sub->AddOption<NumberOption<float>>("Width", nullptr, &g_UiManager->m_Width, 0.01f, 1.f, 0.01f, 2);
				sub->AddOption<BoolOption<bool>>("Sounds", nullptr, &g_UiManager->m_Sounds, BoolDisplay::OnOff);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Links", SUbmenuSettingsLinks, [&](RegularSubmenu* sub)
			{
				
				sub->AddOption<RegularOption>("Zyko Official Website", "Opens Your Browser", [&]
					{
						ShellExecuteA(0, 0, "https://zykocheats.org/", 0, 0, SW_SHOW);
					});
				sub->AddOption<RegularOption>("Zyko Discord", "Opens Your Browser", [&]
					{
						ShellExecuteA(0, 0, "https://discord.gg/GrEkpRQcQE", 0, 0, SW_SHOW);
					});

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Infobar", SubmenuSettingsSubmenuBar, [&](RegularSubmenu* sub)
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

		g_UiManager->AddSubmenu<RegularSubmenu>("Options", SubmenuSettingsOption, [&](RegularSubmenu* sub)
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

		g_UiManager->AddSubmenu<RegularSubmenu>("Footer", SubmenuSettingsFooter, [&](RegularSubmenu* sub)
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

		g_UiManager->AddSubmenu<RegularSubmenu>("Header", SubmenuSettingsHeader, [&](RegularSubmenu* sub)
			{
				sub->AddOption<ChooseOption<const char*, std::size_t>>("Type", nullptr, &Lists::HeaderTypesFrontend, &Lists::HeaderTypesPosition, true, [&]
					{
						g_UiManager->m_HeaderType = Lists::HeaderTypesBackend[Lists::HeaderTypesPosition];
					});
				
				

				sub->AddOption<SubOption>("Text", nullptr, SubmenuSettingsHeaderText);
				sub->AddOption<NumberOption<float>>("Height", nullptr, &g_UiManager->m_HeaderHeight, 0.01f, 0.2f, 0.001f, 3);
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Background", SubmenuSettingsHeaderStaticBackground, [&](RegularSubmenu* sub)
			{
				sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderBackgroundColor.r, '\0', static_cast<std::uint8_t>(255));
				sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderBackgroundColor.g, '\0', static_cast<std::uint8_t>(255));
				sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderBackgroundColor.b, '\0', static_cast<std::uint8_t>(255));
				sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderBackgroundColor.a, '\0', static_cast<std::uint8_t>(255));
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Header Gradient", SubmenuSettingsHeaderGradientBackground, [&](RegularSubmenu* sub)
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
		/*
		g_UiManager->AddSubmenu<RegularSubmenu>("Header Text", SubmenuSettingsHeaderText, [&](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_UiManager->m_HeaderText, BoolDisplay::OnOff);
			sub->AddOption<NumberOption<float>>("Size", nullptr, &g_UiManager->m_HeaderTextSize, 0.1f, 2.f, 0.01f, 2);
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});
		*/
		g_UiManager->AddSubmenu<RegularSubmenu>("Info Windows", SettingsInfoWindows, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("FPS Window", nullptr, &features::fps, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Menu Watermark", nullptr, &features::watermark, BoolDisplay::OnOff);

			});

		}

		/*g_UiManager->AddSubmenu<RegularSubmenu>("Players", SubmenuPlayerList, [&](RegularSubmenu* sub)
		{
			for (std::uint32_t i = 0; i < 32; ++i)
			{
				if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
				{
					sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [&]
					{
						features::g_selected_player = i;
					});
				}
			}
		});

		g_UiManager->AddSubmenu<PlayerSubmenu>(&features::g_selected_player, SubmenuSelectedPlayer, [&](PlayerSubmenu* sub)
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
		g_MainScript.gui_init();
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
