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
	/*Controls*/
	inline bool numpadcontrol = false;

	/*Playerinfo*/
	/* Self */
	void crash(int player);
	void transactionerror(int player);
	void infloading(int player);
	inline bool teleportgun;
	inline bool angryplanesonplayer = false;
	static bool demo_bool = true;
	inline bool godmode;
	inline bool neverWanted = false;
	inline bool sessioninfo = false;
	inline bool instartenter = false;
	inline bool driveitgun = false;
	void set_rank(int rpvalue);
	inline int level{};
	void notify(const char* text, const char* title, int duration);
	void notify_success(const char* text, const char* title, int duration);
	void notify_error(const char* text, const char* title, int duration);
	void notify_protections(const char* title, const char* text, int duration);
	void set_crew_rank(int rpvalue);
	void teleport(int x, int y, int z);
	void spawn_obj(const char* object);
	void play_particle(const char* particle);
	void maxvehicle(int VehicleHandle);
	void setOpenKey();
	inline bool notifyadmin;
	inline bool detectionnotify;
	inline bool leaveondetect;
	inline bool crashgame;
	inline bool troll = true;
	//const char* adminname = "";
	inline bool croshair = false;
	inline bool superman = false;
	inline bool playeresp = false;
	inline bool fucktheircam = false;
	inline bool bullshark = false;
	inline bool aimbot = false;
	inline bool selfdrop = false;
	inline bool deletegun = false;
	inline bool offradar = false;
	inline bool novehkick = false;
	inline bool fixloop = false;
	inline bool nightvision = false;
	inline bool thermalvision = false;
	inline bool exploammo = false;
	inline bool cleanloop = false;
	inline bool speedbypass = true;
	inline bool noclip = false;;
	inline int crew_level{};
	inline int SessionInfoX = 0.020f;
	inline int SessionInfoText = 0.4f;
	inline bool speedometer = true;
	inline bool loadingtext = true;
	inline float timescale;
	inline bool fieldov;
	inline int fieldofview;
	inline bool seatbelt = false;
	inline bool airstrikegun = false;
	inline bool moneynotify = true;
	inline bool modifytimecycle = false;
	inline int playeropacity = 255;
	inline bool trafficfollowplayer = false;
	inline bool spectateplayer = false;
	inline bool nowater = false;
	inline bool forcefield = false;
	inline int worldgravity = 0;
	inline float rainlevelint = 0.0;
	inline int g_HandlingTorque = 1;
	inline int wantedLevel;
	inline int vehiclespeed = 100;
	inline bool stoptime = false;
	inline int HoursTime = 12;
	inline int MinutesTime = 12;
	inline bool superjumpbool = false;
	inline bool infiniteammo = false;
	inline int clearradius = 300;
	inline bool smoothhornboost = false;
	inline bool superrunbool = false;
	inline bool ultrajumpbool = false;
	inline bool ultrarunbool = false;
	inline bool hornboost = false;
	inline bool hornboosteffect = false;
	inline int hornboostvalue = 50;
	inline bool unlimitedstamina = false;
	inline bool flashrun = false;
	inline bool mobileradio = false;
	inline bool nophone = false;
	inline float runspeed = 3.0;
	inline float runspeed1 = 3.0;
	inline int red = 0;
	inline int green = 0;
	inline int blue = 0;
	inline int red2 = 0;
	inline int green2 = 0;
	inline int blue2 = 0;
	inline bool requestmodifier;

	/*Vehicle spawner*/
	void spawn_veh(Hash hash);
	inline bool vehicle_godmode = false;
	inline bool vehicle_blip = false;
	inline bool in_vehicle = true;
	inline bool full_stats = true;

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
	/*PROTECTIONS*/
	inline bool g_received_event = true;
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
	inline std::string g_username = "";
	inline int g_ip_address[4] = { 109, 237, 104, 255 };
	inline uint64_t g_rockstar_id;
	void basket_transaction(int cat, int action, int flag, std::vector<std::array<int, 5>> items);
	inline uint64_t rid = 0;
}
