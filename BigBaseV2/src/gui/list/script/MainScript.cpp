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
			sub->AddOption<RegularOption>("Switch GUI", "Switches to regular UI.", []
			{
				g_list = false;
				g_gui.m_opened = true;
			});
			sub->AddOption<RegularOption>("Unload", "Unload the menu.", []
			{
				g_running = false;
			});
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


		g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [](RegularSubmenu* sub)
		{
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

		g_UiManager->AddSubmenu<RegularSubmenu>("Players", SubmenuPlayerList, [](RegularSubmenu* sub)
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
		});
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
