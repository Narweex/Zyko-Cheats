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
#include "helpers/other.h"
#include "Translation.hpp"
#include "../../BigBaseV2/src/gui/player_list.h"
#include <shellapi.h>
#include "thread_pool.hpp"
#include "../../BigBaseV2/src/gui/list/playerinfo.h"

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
		RockstarAdminSubmenu
	};

	void MainScript::gui_init()
	{
		register_ytd();
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
				sub->AddOption<RegularOption>("Recovery", "Switches to regular UI.", []
					{
						g_list = false;
						g_gui.m_opened = true;
					});
				//sub->AddOption<SubOption>("Recovery", "Recovery Options", recovery);
				sub->AddOption<SubOption>("Misc Options", "Other Options", misc);
				sub->AddOption<SubOption>("Protections", "Protection Options", Protections);
				sub->AddOption<SubOption>("Settings", "Menu settings", SubmenuSettings);

			});

		g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Self Options"), SubmenuSelf, [](RegularSubmenu* sub)
			{
				
				sub->AddOption<SubOption>("Visions", "Change Player Visions", vis);
				sub->AddOption<SubOption>("Model Changer", "Change your Model", SubmenuModelChanger);
				sub->AddOption<BoolOption<bool>>("God Mode", "You Cannot Die", &features::godmode, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Semi God Mode", "Cant Be Detected But Doesnt Work Against Explosions", &features::semigod, BoolDisplay::OnOff);

				sub->AddOption<BoolOption<bool>>("Noclip", "WASD - Direction | SPACE - Up | Shift - Down", &features::noclip, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Never Wanted", "Police Wont Start Coming After You", &features::neverWanted, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Instantly Enter Vehicles", "Come On Lets GO!!!", &features::instartenter, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<int>>("Wanted Level", nullptr, &features::wantedLevel, 0, 5, 1, 3, true, "< ", " >", []
					{
						PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), features::wantedLevel, false);
						PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
					});


				sub->AddOption<BoolOption<bool>>("Super Run - Override", "Run Boiiiiiii", &features::superrunbool, BoolDisplay::OnOff);
				if (features::superrunbool) {
					sub->AddOption<NumberOption<float>>("Run Speed", nullptr, &features::runspeed, 0.1, 5.0, 0.1, 3, true, "< ", " >", [] {});
				}
				sub->AddOption<BoolOption<bool>>("Super Run - Force", "Run Boiiiiiii", &features::ultrarunbool, BoolDisplay::OnOff);
				if (features::ultrarunbool) {
					sub->AddOption<NumberOption<float>>("Run Speed", nullptr, &features::runspeed1, 0.1, 5.0, 0.1, 3, true, "< ", " >", [] {});
				}
				sub->AddOption<BoolOption<bool>>("No Ragdoll", "You are invisible", &features::norag, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Flash Mode", "Run Like Flash", &features::flashrun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Super Jump", "Ever wanted to jump higher?", &features::superjumpbool, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Ultra Jump", "Jump Really High", &features::ultrajumpbool, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Forcefield", "Push everyone", &features::forcefield, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Super Man", "Just Fly", &features::superman, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Off Radar", "Players wont see you on minimap", &features::offradar, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Bullshark Testosterone", "Players wont see you on minimap", &features::bullshark, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Modify Time Scale", "Speed Of The Whole World", &features::modifytimecycle, BoolDisplay::OnOff);
				if (features::modifytimecycle)
				{


					sub->AddOption<NumberOption<float>>("Time Scale", "You Can Do Slow Motion", &features::timescale, 0.0, 1.0, 0.1, 1, true, "< ", " >", [] {

						});
				}
				sub->AddOption<RegularOption>("Clear Wanted", "Clear Player Wanted Level", []
					{
						PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
					});

				sub->AddOption<RegularOption>("Max Health", "Adds Health To The Player", []
					{
						features::maxhealth();
					});
				sub->AddOption<RegularOption>("Max Armour", "Adds Armour To The Player", []{features::maxarmor();});
				sub->AddOption<BoolOption<bool>>("Invisibility", "You are invisible", &features::invisibility, BoolDisplay::OnOff);

				sub->AddOption<NumberOption<int>>("Player Opacity", "Visibility Of Your Player", &features::playeropacity, 0, 255, 1, 3, true, "< ", " >", [] {
					ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), features::playeropacity, 0);
					});
				sub->AddOption<BoolOption<bool>>("Infinite Stamina", "You Can Just Run Forever", &features::unlimitedstamina, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clean Ped", "Clean Player", []
					{
						PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID());
					});
				sub->AddOption<RegularOption>("Randomise Clothes", "Randomises Players Clothes", []
					{
						PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), true);
					});
				sub->AddOption<RegularOption>("Reset Ped", "Reset Player", [] {features::resetped();});
				sub->AddOption<RegularOption>("Suicide", "Kills Ped", []{features::suicide();});
				/*sub->AddOption<RegularOption>(xorstr_("RegularOption_demo"), "A regular option.", []
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
			g_UiManager->AddSubmenu<RegularSubmenu>(xorstr_("Model Changer"), SubmenuModelChanger, [](RegularSubmenu* sub)
				{
					for(auto& model:Lists::models1)
					{ 
						sub->AddOption<RegularOption>(model, "Clears Vision", [model]
							{
								features::changemodel(model);
							});
					}
					
				});
		g_UiManager->AddSubmenu<RegularSubmenu>("Visions", vis, [](RegularSubmenu* sub)
			{

				sub->AddOption<BoolOption<bool>>("Night Vision", "You can see cool in night", &features::nightvision, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Thermal Vision", "Run Boiiiiiii", &features::thermalvision, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clear", "Clears Vision", []
					{
						GRAPHICS::SET_TIMECYCLE_MODIFIER("li");;
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
		g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Movement", SubmenuVehicleMovement, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Bypass Max Speed", "Cleans Vehicle Automatically When Dirty", &features::speedbypass, BoolDisplay::OnOff);

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
						sub->AddOption<NumberOption<int>>("Horn Boost Speed", nullptr, &features::hornboostvalue, 0, 200, 10, 3, true, "< ", " >", [] {});
					}
					sub->AddOption<BoolOption<bool>>("Horn Boost Effect", "Adds a Pog Turbo Effect", &features::hornboosteffect, BoolDisplay::OnOff);
				}
				sub->AddOption<NumberOption<std::int32_t>>("Vehicle Speed", nullptr, &features::vehiclespeed, 0, 500, 10, 3, true, "< ", " >", [] {});
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
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~r~R~s~]", nullptr, &features::red, 0, 255, 10, 3, true, "< ", " >", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~g~G~s~]", nullptr, &features::green, 0, 255, 10, 3, true, "< ", " >", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Primary [~b~B~s~]", nullptr, &features::blue, 0, 255, 10, 3, true, "< ", " >", [] {});
				sub->AddOption<RegularOption>("Set Vehicle Primary RGB", "Random Paint Vehicle", []
					{
						int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, features::red, features::green, features::blue);
					});
				sub->AddOption<NumberOption<std::int32_t>>("Color Secondary [~r~R~s~]", nullptr, &features::red2, 0, 255, 10, 3, true, "< ", " >", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Secondary [~g~G~s~]", nullptr, &features::green2, 0, 255, 10, 3, true, "< ", " >", [] {});
				sub->AddOption<NumberOption<std::int32_t>>("Color Secondary [~b~B~s~]", nullptr, &features::blue2, 0, 255, 10, 3, true, "< ", " >", [] {});
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
				sub->AddOption<BoolOption<bool>>("Vehicle Godmode", nullptr, &features::vehgodmode, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Max Tuning", "Tune the Vehicle To Maximum", []
					{
						int Vehicle;
						Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(features::g_selected_player), 0);
						features::maxvehicle(Vehicle);
					});
				sub->AddOption<RegularOption>("Repair Vehicle", "Repair Damages To Currnet Vehicle", []
					{
						VEHICLE::SET_VEHICLE_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
						VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())));
					});
				sub->AddOption<BoolOption<bool>>("Repair Vehicle Loop", "Repair Vehicle Automatically When Demaged", &features::fixloop, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Seatbelt", "You Cant Fall Of Vehicle", &features::seatbelt, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Duplicate Current Vehicle", "Spawn Another One", []
					{
						features::duplicatecar();
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
				sub->AddOption<BoolOption<bool>>("Clean Vehicle Loop", "Cleans Vehicle Automatically When Dirty", &features::cleanloop, BoolDisplay::OnOff);

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
		g_UiManager->AddSubmenu<RegularSubmenu>("Spawner Settings", SubmenuVehSpawnerSettings, [&](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Spawn Invincible", "Vehicle Will Be Spawned With Godmode", &features::vehicle_godmode, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn Inside", "You Will Be Spawned Inside Vehicle", &features::in_vehicle, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn Max Tuned", "Vehicle Will HAve Max Tuning", &features::full_stats, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Spawn With Blip", "You Will See The Vehicle On Map", &features::vehicle_blip, BoolDisplay::OnOff);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Sports", SubmenuVehSpawnerSports, [&](RegularSubmenu* sub)
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
				sub->AddOption<RegularOption>("RID test", nullptr, []
					{
						static bool done = false;

						g_thread_pool->push([&] //thread pool for not making fps lag
							{
								features::rid = get_rid_from_name("Yodo2222");
								done = true;
							});

						if (done)
							LOG(INFO) << features::rid;

						// You can use keyboard get input func for getting target name and then display it on option text
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("All Players", SessionAllPlayersSubmenu, [&](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Teleport", "All players in session", AllPlayerTeleport);
				sub->AddOption<SubOption>("Abuse", "You can mess with all of them in once", AllPlayerAbuse);
				sub->AddOption<SubOption>("Vehicle", "Modify The Session", AllPlayerVehicle);
				sub->AddOption<SubOption>("Trolling", "You can mess with all of them in once", AllPlayerTrolling);
				sub->AddOption<SubOption>("Friendly", "Modify The Session", AllPlayerFriendly);
				sub->AddOption<BoolOption<bool>>("ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
				if (features::playeresp)
				{

					sub->AddOption<BoolOption<bool>>("Line ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
					sub->AddOption<BoolOption<bool>>("Box ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
					//sub->AddOption<BoolOption<bool>>("ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
					//sub->AddOption<BoolOption<bool>>("ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
					//sub->AddOption<BoolOption<bool>>("ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
					//sub->AddOption<BoolOption<bool>>("ESP", "You will get guided where other players are", &features::playeresp, BoolDisplay::OnOff);
				}
				g_UiManager->AddSubmenu<RegularSubmenu>("Player Teleports", AllPlayerTeleport, [](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

						sub->AddOption<RegularOption>("Teleport To Player", "tp to Player", []
							{
								Entity handle;
								PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
								ENTITY::SET_ENTITY_COORDS(handle, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).z, false, false, false, false);
							});
						sub->AddOption<RegularOption>("Teleport To Player's Vehicle", "tp to Players vehicle", []
							{
								if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(NULL), true))
								{
									PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(features::g_selected_player)), VEHICLE::IS_VEHICLE_SEAT_FREE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(NULL)), -1, true));
								}
							});
					});

				g_UiManager->AddSubmenu<RegularSubmenu>("Abuse", AllPlayerAbuse, [](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

						if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == PLAYER::PLAYER_PED_ID())
						{
							sub->AddOption<RegularOption>("Host Kick", "Kick From Session", []
								{
									NETWORK::NETWORK_SESSION_KICK_PLAYER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
									int hostkick[69] = { 1885259, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) };
									SCRIPT::TRIGGER_SCRIPT_EVENT(1, hostkick, 4, 1 << features::g_selected_player);
									//1885259 host kick global
								});
						}
						sub->AddOption<RegularOption>("Kick", "Kick ", []
							{
								for (int i = 0; i < 32; i++)
								{
									features::kick(i);
								}

							});
						sub->AddOption<RegularOption>("Nig Nog Crash", "Nigs them out from game", []
							{

								for (int i = 0; i < 32; i++)
								{
									features::crash(i);
								}

							});
						sub->AddOption<RegularOption>("Test Crash", "Test Crash From Session", []
							{

								for (int i = 0; i < 32; i++)
								{
									features::infloading(i);
								}

							});
						sub->AddOption<RegularOption>("Script Kick", "Kick From Session", []
							{
								std::uint64_t args_1[] = { -145306724, 1, 0, 0 };

								g_pointers->m_TriggerScriptEvent(1, args_1, 4, NULL);

							});
					});
				g_UiManager->AddSubmenu<RegularSubmenu>("Players Vehicle Options", AllPlayerVehicle, [](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);


					});

				g_UiManager->AddSubmenu<RegularSubmenu>("Trolling", AllPlayerTrolling, [](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

						sub->AddOption<RegularOption>("Clone Player", "Clones Player", []
							{
								PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1, 1, 1);
							});
						sub->AddOption<RegularOption>("Attach To Player", "Attaches You To Player", []
							{
								if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) == PLAYER::PLAYER_PED_ID())
								{

								}
								else
								{
									ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, false, true);
								}
							});
						sub->AddOption<RegularOption>("Detach Off Player", "Detach You From Player", []
							{
								ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), true, true);
							});
						sub->AddOption<RegularOption>("Explode Player", "Blow Up Player", []
							{
								Vector3 targetCords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
								FIRE::ADD_EXPLOSION(targetCords.x, targetCords.y, targetCords.z, 29, 9999.0f, true, false, 0.0f, false);
							});
						sub->AddOption<RegularOption>("Give Him Wanted Level", "Give The Player Wanted Level", []
							{
								PLAYER::SET_PLAYER_WANTED_LEVEL(features::g_selected_player, 5, false);
								PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
							});
						sub->AddOption<BoolOption<bool>>("Fuck Their Camera", "Just a Little Trolling", &features::fucktheircam, BoolDisplay::OnOff);
						sub->AddOption<BoolOption<bool>>("Traffic Follows Player", "This Will Piss Them Off", &features::trafficfollowplayer, BoolDisplay::OnOff);
						sub->AddOption<RegularOption>("Airstrike Player", "Blow Up Player With Airstrike", []
							{
								Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
								Ped playerPed = PLAYER::PLAYER_PED_ID();
								Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
								Hash airStrike = rage::joaat("WEAPON_AIRSTRIKE_ROCKET");
								WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
								MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 50.f, coords.x, coords.y, coords.z, 250, 1, airStrike, playerPed, 1, 0, -1.0);

							});
						sub->AddOption<RegularOption>("Cage Player", "Trap In cage", []
							{
								for (int i = 0; i < 32; i++)
								{
									OBJECT::CREATE_OBJECT(959275690, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i), false).z - 1.f, true, false, false);
								}
							});
						sub->AddOption<RegularOption>("Tube Player", "Trap In Tube", []
							{
								Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0);
								Object obj = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("stt_prop_stunt_tube_fn_01"), remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, true);
								ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 90.0f, 0.0f, 0, true);
							});
						sub->AddOption<RegularOption>("Tree Player", "Trap In Tree", []
							{
								Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
								Object obj1 = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("prop_xmas_tree_int"), pos.x, pos.y, pos.z, 1, 1, 1);
							});

					});

				g_UiManager->AddSubmenu<RegularSubmenu>("Friendly", AllPlayerFriendly, [](RegularSubmenu* sub)
					{
						GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

					});

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


				sub->AddOption<RegularOption>("Remove Weapons", "Remove Weapons", []
					{
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID(), true);
					});
				//sub->AddOption<BoolOption<bool>>("Always Crosshair", "Bruh Read The Title", &features::croshair, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Infinite ammo", "Your gun will never be empty", &features::infiniteammo, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Delete Gun", "Your gun will never be empty", &features::deletegun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Aimbot", "Are you a noob?", &features::aimbot, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Explosive Gun", "Your gun will never be empty", &features::exploammo, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Airstrike Gun", "A Plane Will Strike Where You Shoot", &features::airstrikegun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Teleport Gun", "You Will Teleport Where You Shoot", &features::teleportgun, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Drive It Gun", "You Will Enter The Vehicle YOu Shoot At", &features::driveitgun, BoolDisplay::OnOff);


			});

		g_UiManager->AddSubmenu<RegularSubmenu>("World_Options", WorldOptions, [](RegularSubmenu* sub)
			{
				sub->AddOption<SubOption>("Time", "Adjust the World Time", TimeSubmenu);
				sub->AddOption<SubOption>("Weather", "Adjust the World Time", WeatherSubmenu);
				//sub->AddOption<SubOption>("Water", "Mess With Water", WaterSubmenu);
				sub->AddOption<SubOption>("Clear Area", "Select what to clear from you", ClearSubmenu);
				sub->AddOption<NumberOption<std::int32_t>>("World Gravity", nullptr, &features::worldgravity, 0, 3, 1, 3, true, "< ", " >", [] {});
				sub->AddOption<RegularOption>("Set Gravity", "", []
					{
						MISC::SET_GRAVITY_LEVEL(features::worldgravity);
					});
				sub->AddOption<NumberOption<float>>("Rain Intensity", nullptr, &features::rainlevelint, 0, 1000, 10, 3, true, "< ", " >", [] {});
				sub->AddOption<RegularOption>("Set Intensity", "", []
					{
						MISC::_SET_RAIN_FX_INTENSITY(features::rainlevelint);
					});
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Time", TimeSubmenu, [](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Stop Time", "Freezes Time In the World", &features::stoptime, BoolDisplay::OnOff);
				sub->AddOption<NumberOption<int>>("Hours", "Might Not Work In Online!", &features::HoursTime, 0, 23, 1, 1, true, "< ", " >", []
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, 0, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, 0, 0);
					});
				sub->AddOption<NumberOption<int>>("Minutes", "Might Not Work In Online!", &features::MinutesTime, 0, 59, 1, 1, true, "< ", " >", []
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
				sub->AddOption<NumberOption<int>>("Hours", "Might Not Work In Online!", &features::HoursTime, 0, 23, 1, 1, true, "< ", " >", []
					{
						CLOCK::SET_CLOCK_TIME(features::HoursTime, 0, 0);
						NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(features::HoursTime, 0, 0);
					});
				sub->AddOption<NumberOption<int>>("Minutes", "Might Not Work In Online!", &features::MinutesTime, 0, 59, 1, 1, true, "< ", " >", []
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

				sub->AddOption<NumberOption<int>>("Radius", "How far should objects be cleared?", &features::clearradius, 1, 500, 10, 1, true, "< ", " >", []
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
				sub->AddOption<RegularOption>("Teleport To Objective ", "Teleport to Objective", [] { {features::tpobjective(); }});
				sub->AddOption<RegularOption>("show coords ", "ok", []
					{
						Vector3 neger = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
						float iks = neger.x;
						float yps = neger.y;
						float zet = neger.z;
						features::notify("idk", "idk", 20000);
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
				playerinfo::render_playerinfo();
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				for (std::uint32_t i = 0; i < 32; ++i)
				{
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
					{
						if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == i)
						{
							char Buffer[255]; sprintf(Buffer, "%s   ~p~[SESSION HOST]", PLAYER::GET_PLAYER_NAME(i));  /////// HOST 
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									features::g_selected_player = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == PLAYER::PLAYER_PED_ID())
						{
							char Buffer[255]; sprintf(Buffer, "%s   ~b~[SELF]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									features::g_selected_player = i;
								});
						}
						else if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i) == NETWORK::NETWORK_IS_FRIEND_IN_MULTIPLAYER(PLAYER::GET_PLAYER_NAME(i))) {
							char Buffer[255]; sprintf(Buffer, "%s   ~g~[FRIEND]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									features::g_selected_player = i;
								});
						}
						else if (g_player_list.is_modder) {
							char Buffer[255]; sprintf(Buffer, "%s   ~r~[MODDER]", PLAYER::GET_PLAYER_NAME(i));
							sub->AddOption<SubOption>(Buffer, nullptr, SubmenuSelectedPlayer, [=]
								{
									features::g_selected_player = i;
								});
						}
						// 
						else
						{
							sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [=]
								{
									features::g_selected_player = i;
								});
						}
					}
				}
			});

		g_UiManager->AddSubmenu<PlayerSubmenu>(&features::g_selected_player, SubmenuSelectedPlayer, [](PlayerSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);
				//list_playerinfo::render_playerinfo();

				sub->AddOption<BoolOption<bool>>("Spectate", nullptr, &features::spectateplayer, BoolDisplay::OnOff);
				sub->AddOption<SubOption>("Teleport Options", nullptr, SelectedPlayerTeleport);
				sub->AddOption<SubOption>("Vehicle Options", nullptr, SelectedPlayerVehicle);
				sub->AddOption<SubOption>("Abuse", nullptr, SelectedPlayerAbuse);
				sub->AddOption<SubOption>("Trolling Options", nullptr, SelectedPlayerTrolling);
				sub->AddOption<SubOption>("Friendly Options", nullptr, SelectedPlayerFriendly);
			});




		g_UiManager->AddSubmenu<RegularSubmenu>("Player Teleports", SelectedPlayerTeleport, [](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				sub->AddOption<RegularOption>("Teleport To Player", "tp to Player", []
					{
						Entity handle;
						PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : handle = PLAYER::PLAYER_PED_ID();
						ENTITY::SET_ENTITY_COORDS(handle, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).z, false, false, false, false);
					});
				sub->AddOption<RegularOption>("Teleport To Player's Vehicle", "tp to Players vehicle", []
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED(NULL), true))
						{
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(features::g_selected_player)), VEHICLE::IS_VEHICLE_SEAT_FREE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(NULL)), -1, true));
						}
					});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Abuse", SelectedPlayerAbuse, [](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				if (NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0) == PLAYER::PLAYER_PED_ID())
				{
					sub->AddOption<RegularOption>("Host Kick", "Kick From Session", []
						{
							NETWORK::NETWORK_SESSION_KICK_PLAYER(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player));
							int hostkick[69] = { 1885259, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) };
							SCRIPT::TRIGGER_SCRIPT_EVENT(1, hostkick, 4, 1 << features::g_selected_player);
							//1885259 host kick global
						});
				}
				sub->AddOption<RegularOption>("Kick", "Kick ", []
					{
						features::kick(features::g_selected_player);
					});
				sub->AddOption<RegularOption>("Nig Nog Crash", "Crash From Game", []
					{

						features::crash(features::g_selected_player);

					});
				sub->AddOption<RegularOption>("Infinite Loading", "He Will be In Loadingscreen Forever", []
					{

						features::infloading(features::g_selected_player);

					});
				sub->AddOption<RegularOption>("Transaction error", "They will be in infinite transaction", []
					{

						features::transactionerror(features::g_selected_player);

					});

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Players Vehicle Options", SelectedPlayerVehicle, [](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);


			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Trolling", SelectedPlayerTrolling, [](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

				sub->AddOption<BoolOption<bool>>("Angry Planes", "Spawns a bunch of planes that will follow and kill the player", &features::angryplanesonplayer, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Clone Player", "Clones Player", []
					{
						PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 1, 1, 1);
					});
				sub->AddOption<RegularOption>("Attach To Player", "Attaches You To Player", []
					{
						if (PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player) == PLAYER::PLAYER_PED_ID())
						{

						}
						else
						{
							ENTITY::ATTACH_ENTITY_TO_ENTITY(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, true, true, false, true, false, true);
						}
					});
				sub->AddOption<RegularOption>("Detach Off Player", "Detach You From Player", []
					{
						ENTITY::DETACH_ENTITY(PLAYER::PLAYER_PED_ID(), true, true);
					});
				sub->AddOption<RegularOption>("Explode Player", "Blow Up Player", []
					{
						Vector3 targetCords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
						FIRE::ADD_EXPLOSION(targetCords.x, targetCords.y, targetCords.z, 29, 9999.0f, true, false, 0.0f, false);
					});
				sub->AddOption<RegularOption>("Give Him Wanted Level", "Give The Player Wanted Level", []
					{
						PLAYER::SET_PLAYER_WANTED_LEVEL(features::g_selected_player, 5, false);
						PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
					});
				sub->AddOption<BoolOption<bool>>("Fuck Their Camera", "Just a Little Trolling", &features::fucktheircam, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Traffic Follows Player", "This Will Piss Them Off", &features::trafficfollowplayer, BoolDisplay::OnOff);
				sub->AddOption<RegularOption>("Airstrike Player", "Blow Up Player With Airstrike", []
					{
						Ped selectedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(NULL);
						Ped playerPed = PLAYER::PLAYER_PED_ID();
						Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedplayer, 1);
						Hash airStrike = rage::joaat("WEAPON_AIRSTRIKE_ROCKET");
						WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords.x, coords.y, coords.z + 50.f, coords.x, coords.y, coords.z, 250, 1, airStrike, playerPed, 1, 0, -1.0);

					});
				sub->AddOption<RegularOption>("Cage Player", "Trap In cage", []
					{
						OBJECT::CREATE_OBJECT(959275690, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false).z - 1.f, true, false, false);
					});
				sub->AddOption<RegularOption>("Tube Player", "Trap In Tube", []
					{
						Vector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), 0);
						Object obj = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("stt_prop_stunt_tube_fn_01"), remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, true);
						ENTITY::SET_ENTITY_ROTATION(obj, 0.0f, 90.0f, 0.0f, 0, true);
					});
				sub->AddOption<RegularOption>("Tree Player", "Trap In Tree", []
					{
						Vector3 pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true);
						Object obj1 = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("prop_xmas_tree_int"), pos.x, pos.y, pos.z, 1, 1, 1);
					});

			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Friendly", SelectedPlayerFriendly, [](RegularSubmenu* sub)
			{
				GRAPHICS::DRAW_MARKER(2, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), true).z + 1.25, 0, 0, 0, 0, 180, 0, 0.35, 0.35, 0.35, 200, 0, 100, 255, 1, 1, 1, 0, 0, 0, 0);

			});





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
				sub->AddOption<SubOption>("Object Spawner", nullptr, SubmenuSettingsObjectSpanwer);
				//sub->AddOption<SubOption>("Particles", nullptr, SubmenuSettingsParticles);
				//sub->AddOption<BoolOption<bool>>("Money Drop On Self [~r~RISKY]", "Vibe to the music everywhere!", &features::selfdrop, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Mobile Radio", "Vibe to the music everywhere!", &features::mobileradio, BoolDisplay::OnOff);
				//sub->AddOption<BoolOption<bool>>("Free Camera", "Vibe to the music everywhere!", &features::, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Disable Phone", "English Dave wont bother you", &features::nophone, BoolDisplay::OnOff);


				sub->AddOption<RegularOption>("Skip Cutscene", "Skips current cutscene", []
					{

						CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
					});

				//sub->AddOption<RegularOption>("Break Session test", "test", []
				//	{
				//		MISC::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("Freemode");
				//		//
				//	});
			});

		g_UiManager->AddSubmenu<RegularSubmenu>("Miscellaneous", misc, [](RegularSubmenu* sub)
			{
				sub->AddOption<BoolOption<bool>>("Money Drop On Self [~r~RISKY]", "Vibe to the music everywhere!", &features::selfdrop, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Mobile Radio", "Vibe to the music everywhere!", &features::mobileradio, BoolDisplay::OnOff);
				//sub->AddOption<BoolOption<bool>>("Free Camera", "Vibe to the music everywhere!", &features::freecam, BoolDisplay::OnOff);
				sub->AddOption<BoolOption<bool>>("Disable Phone", "English Dave wont bother you", &features::nophone, BoolDisplay::OnOff);

			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Protections", Protections, [](RegularSubmenu* sub)
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

		g_UiManager->AddSubmenu<RegularSubmenu>("Language", SubmenuSettingsLanguage, [](RegularSubmenu* sub)
			{
				std::filesystem::directory_iterator dirIt{ g_TranslationManager->GetTranslationDirectory() };
				for (auto&& dirEntry : dirIt)
				{

					auto path = dirEntry.path();
					if (path.has_filename())
					{
						sub->AddOption<RegularOption>(path.stem().u8string().c_str(), nullptr, [=]
							{
								g_TranslationManager->LoadTranslations(path.stem().u8string().c_str());
							});
					}

				}



			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Object Spawner", SubmenuSettingsObjectSpanwer, [](RegularSubmenu* sub)
			{
				for (auto& obj : Lists::Objects1) {
					sub->AddOption<RegularOption>(obj, "Spawn this object", [obj]
						{
							features::spawn_obj(obj);
						});
				}

			});
		/*g_UiManager->AddSubmenu<RegularSubmenu>("Particles", SubmenuSettingsParticles, [](RegularSubmenu* sub)
			{
				for (auto& particle : Lists::Particles1) {
					sub->AddOption<RegularOption>(particle, "Play This Particle", [particle]
						{
							features::play_particle(particle);
						});
				}

			});*/



		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [](RegularSubmenu* sub)
			{
				/*sub->AddOption<RegularOption>("Switch GUI", "Switches to regular UI.", []
					{
						g_list = false;
						g_gui.m_opened = true;
					});*/
					/*sub->AddOption<RegularOption>("notify", "Unload the menu.", []
						{
							features::notify_protections("Test", "Notification", 4000);
						});
					sub->AddOption<RegularOption>("error", "Unload the menu.", []
						{
							features::notify_error("An error occured", "Notification", 4000);
						});*/

				sub->AddOption<BoolOption<bool>>("Numpad Control", " 8 - Up | 2 - Down | 4 - Left | 6 - Right | 5 - Select |\n 0 - Back", &features::numpadcontrol, BoolDisplay::OnOff);


				
				sub->AddOption<RegularOption>("Unload", "Unload the menu.", []
					{
						g_running = false;
					});
				
				//sub->AddOption<SubOption>("Header", nullptr, SubmenuSettingsHeader);
				sub->AddOption<SubOption>("Infobar", nullptr, SubmenuSettingsSubmenuBar);
				sub->AddOption<SubOption>("Links", nullptr, SUbmenuSettingsLinks);
				sub->AddOption<SubOption>("Options", nullptr, SubmenuSettingsOption);
				sub->AddOption<SubOption>("Footer", nullptr, SubmenuSettingsFooter);
				sub->AddOption<SubOption>("Description", nullptr, SubmenuSettingsDescription);
				//sub->AddOption<SubOption>("Input", nullptr, SubmenuSettingsInput);
				sub->AddOption<SubOption>("Language", nullptr, SubmenuSettingsLanguage);
				sub->AddOption<NumberOption<float>>("X Position", nullptr, &g_UiManager->m_PosX, 0.f, 1.f, 0.01f, 2);
				sub->AddOption<NumberOption<float>>("Y Position", nullptr, &g_UiManager->m_PosY, 0.f, 1.f, 0.01f, 2);
				sub->AddOption<NumberOption<float>>("Width", nullptr, &g_UiManager->m_Width, 0.01f, 1.f, 0.01f, 2);
				sub->AddOption<BoolOption<bool>>("Sounds", nullptr, &g_UiManager->m_Sounds, BoolDisplay::OnOff);
			});
		g_UiManager->AddSubmenu<RegularSubmenu>("Links", SUbmenuSettingsLinks, [](RegularSubmenu* sub)
			{

				sub->AddOption<RegularOption>("Zyko Official Website", "Opens Your Browser", []
					{
						ShellExecuteA(0, 0, "https://zykocheats.org/", 0, 0, SW_SHOW);
					});
				sub->AddOption<RegularOption>("Zyko Discord", "Opens Your Browser", []
					{
						ShellExecuteA(0, 0, "https://discord.gg/GrEkpRQcQE", 0, 0, SW_SHOW);
					});

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
		/*
		g_UiManager->AddSubmenu<RegularSubmenu>("Header Text", SubmenuSettingsHeaderText, [](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_UiManager->m_HeaderText, BoolDisplay::OnOff);
			sub->AddOption<NumberOption<float>>("Size", nullptr, &g_UiManager->m_HeaderTextSize, 0.1f, 2.f, 0.01f, 2);
			sub->AddOption<NumberOption<std::uint8_t>>("R", nullptr, &g_UiManager->m_HeaderTextColor.r, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("G", nullptr, &g_UiManager->m_HeaderTextColor.g, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("B", nullptr, &g_UiManager->m_HeaderTextColor.b, '\0', static_cast<std::uint8_t>(255));
			sub->AddOption<NumberOption<std::uint8_t>>("A", nullptr, &g_UiManager->m_HeaderTextColor.a, '\0', static_cast<std::uint8_t>(255));
		});
		*/
		g_UiManager->AddSubmenu<RegularSubmenu>("Description", SubmenuSettingsDescription, [](RegularSubmenu* sub)
			{
				sub->AddOption<NumberOption<float>>("Padding", "Padding before the description rect.", &g_UiManager->m_DescriptionHeightPadding, 0.01f, 1.f, 0.001f,
					3);
				sub->AddOption<NumberOption<float>>("Height", "Size of the description rect.", &g_UiManager->m_DescriptionHeight, 0.01f, 1.f, 0.001f, 3);
				sub->AddOption<NumberOption<float>>("Text Size", "Size of the description text.", &g_UiManager->m_DescriptionTextSize, 0.1f, 2.f, 0.01f, 2);
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
