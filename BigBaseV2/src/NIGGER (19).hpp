#pragma once
#include "common.hpp"
#include "../../BigBaseV2/src/core/config.h"

namespace big::features
{
	inline int OpenKey = VK_INSERT;
	void run_tick();
	void script_func();
	void on_present();

	inline ULONGLONG ticks[] = { 0, 0, 0, 0 };
	inline ULONGLONG tick_conf[] = { 50, 250, 2000, 25003 };
	inline int g_current_tick_job{};


	void player_loop();
	void online_loop();
	void weapons_loop();
	void vehicle_loop();
	void misc_loop();

	/* Self */
	inline bool godmode;
	inline bool neverWanted = false;
	inline bool instartenter = false;
	inline bool superman = false;
	inline bool noclip = false;
	inline bool nightvision = false;
	inline bool thermalvision = false;
	inline bool invisibility = false;
	inline bool norag = false;
	inline bool semigod = false;
	inline bool bullshark = false;
	inline bool offradar = false;
	inline float timescale;
	inline int fieldofview;
	inline bool modifytimecycle = false;
	inline int playeropacity = 255;
	inline float runspeed = 3.0;
	inline float runspeed1 = 3.0;
	inline bool forcefield = false;
	inline bool unlimitedstamina = false;
	inline bool flashrun = false;
	inline bool mobileradio = false;
	void clearwanted();
	void resetped();
	void suicide();
	void maxhealth();
	void maxarmor();
	void noIdleKick();
	inline bool noidlekick;
	inline bool requestmodifier;
	inline int wantedLevel;
	inline int playeralpha = 255;
	inline bool superrunbool = false;
	inline bool ultrajumpbool = false;
	inline bool ultrarunbool = false;
	inline bool superjumpbool = false;
	inline bool ignoreplayer = false;
	void changemodel(const char* model);
	void setwanted();


	/* Weapons */
	inline bool driveitgun = false;
	inline bool teleportgun;
	inline bool aimbot = false;
	inline bool deletegun = false;
	inline bool airstrikegun = false;
	inline bool exploammo = false;
	void GiveAllWeapons();
	void RemoveAllWeapons();
	void MaxAmmo();
	inline bool infiniteammo = false;


	/* Vehicle */
	void spawn_veh(Hash hash);
	void maxvehicle(int VehicleHandle);
	inline bool vehicle_godmode = false;
	inline bool vehicle_blip = false;
	inline bool in_vehicle = true;
	inline bool full_stats = true;
	inline bool cleanloop = false;
	inline bool speedbypass = true;
	inline bool novehkick = false;
	inline bool fixloop = false;
	inline int red = 0;
	inline int green = 0;
	inline int blue = 0;
	inline int red2 = 0;
	inline int green2 = 0;
	inline int blue2 = 0;
	inline bool vehgodmode = false;
	inline bool hornboost = false;
	inline bool hornboosteffect = false;
	inline int hornboostvalue = 50;
	inline bool seatbelt = false;
	inline int vehiclespeed = 100;
	inline bool smoothhornboost = false;
	inline bool rainbowcar = false;
	void repairVehicle();
	void cleanVehicle();
	void duplicatecar();
	//int maincolour;


	/* Teleport */
	void teleport_to_objective();
	void teleport(int x, int y, int z);
	void teleport_to_waypoint();

	/* Recovery */
	void set_rank(int rpvalue);
	inline int level{};
	void set_crew_rank(int rpvalue);
	void basket_transaction(int cat, int action, int flag, std::vector<std::array<int, 5>> items);
	void startupmoney();
	inline int crew_level{};
	static int bunker_money;
	void addMoney();
	void cayoSkipPreps();
	void cayoHardMode();
	void unlock_everything();
	void unlock_achievements();
	void max_creation_date();
	void eight_years_playtime();

	/* Online */
	inline bool playeresp = false;
	inline bool fucktheircam = false;
	inline bool spectateplayer = false;
	inline bool moneynotify = true;

	/* World*/
	inline int worldgravity = 0;
	inline float rainlevelint = 0.0;
	inline bool stoptime = false;
	inline int HoursTime = 12;
	inline int MinutesTime = 12;
	inline int clearradius = 300;

	/* Utility */
	void notify(const char* text, const char* title, int duration);
	void notify_success(const char* text, const char* title, int duration);
	void notify_error(const char* text, const char* title, int duration);
	void notify_protections(const char* title, const char* text, int duration);
	void spawn_obj(const char* object);
	void play_particle(const char* particle);
	static bool demo_bool = true;
	inline bool numpadcontrol = false;




	/* Misc*/
	void setOpenKey();
	inline bool nophone = false;
	void RequestControlOfEnt(Entity entity);
	void ApplyForceToEntity(Entity e, float x, float y, float z);
	void coordsDisplay();
	void rainbowloop();
	void isDev();
	void check();
	void isRunning(LPCWSTR name);
	
	/* Online */
	inline std::uint32_t g_selected_player{};
	bool is_modder(Player player);
	enum ChangeSessionID
	{
		GTAO = -1,
		FindPublicSession = 0,
		StartNewPublicSession = 1,
		ClosedCrewSession = 2,
		CrewSession = 3,
		ClosedFriendSession = 6,
		FindFriendsSession = 9,
		StartSoloSession = 10,
		InviteOnlySession = 11,
		JoinCrewSession = 12,
		JoinSCTV = 13,
	};
	void change_session(ChangeSessionID ID);

	/*Trolling and kicks*/
	void kick(int player);
	void notification(int player);
	void kill(int player);
	void vehkick(int player);
	void StealWeapons();
	void crash(int player);
	void transactionerror(int player);
	void infloading(int player);

	/* Protections */
	inline bool g_received_event = false;
	inline bool g_explosion_event = false;
	inline bool no_report_event = false;
	inline bool g_ptfx_event = false;
	inline bool g_log_net_event_data = false;
	inline bool no_freeze_event = false;
	inline bool no_pickup_event = false;
	inline bool no_clock_event = false;
	inline bool no_sound_effect = false;
	inline bool no_votekick_event = false;
	inline bool no_weapons_event = false;
	inline bool no_wanted_event = false;
	inline bool no_weather_event = false;
	inline bool g_scripted_game_event = false;
	inline bool g_crash_protex = false;
	inline bool g_kick_protex = false;
	inline bool rockstaradmin = false;
	inline bool g_spoof_username = false;
	inline bool g_spoof_ip = false;
	inline bool g_spoof_rockstar_id = false;
	inline bool leaveondetect;
	inline bool crashgame;
	inline bool notifyadmin;
	void admindetection();
	inline bool detectionnotify = true;
	inline std::string g_username = "Narweex";
	inline int g_ip_address[4] = { 109, 237, 104, 255 };
	inline uint64_t g_rockstar_id;
	
	inline uint64_t rid = 0;

	
	void show_watermark(bool enable);
	void show_info_pool(bool enable);

	inline bool watermark = true, pools = true;
}
