#pragma once
#include "common.hpp"


namespace zyko::features
{
	inline int OpenKey = VK_INSERT;
	void run_tick();
	void script_func();
	void on_present();

	inline ULONGLONG ticks[] = { 0, 0, 0, 0 };
	inline ULONGLONG tick_conf[] = { 50, 2500, 5000, 25003 };
	inline int g_current_tick_job{};



	/* Self */
	extern bool godmode;
	void Godmode(bool toggle);

	extern bool neverwanted;
	void Neverwanted(bool toggle);

	extern bool instantenter;
	void Instantenter(bool toggle);

	extern bool superman;
	void Superman(bool toggle);

	extern bool noclip;
	void Noclip(bool toggle);

	extern bool nightvision;
	void Nightvision(bool toggle);

	extern bool thermalvision;
	void Thermalvision(bool toggle);

	extern bool invisibility;
	void Invisibility(bool toggle);

	extern bool norag;
	void Norag(bool toggle);

	extern bool semigod;
	void Semigod(bool toggle);

	extern bool bullshark;
	void Bullshark(bool toggle);

	extern bool offradar;
	void Offradar(bool toggle);

	extern bool flashrun;
	void Flashrun(bool toggle);

	extern bool mobileradio;
	void Mobileradio(bool toggle);

	extern bool hide_hud;
	void Hide_hud(bool toggle);

	extern bool superjumpbool;
	void Superjump(bool toggle);

	extern bool ultrajumpbool;
	void Ultrajumpbool(bool toggle);

	inline float timescale;

	inline bool modifytimecycle;
	//void Modifytimecycle(bool toggle);

	extern bool forcefield;
	inline float forcefield_force = 5.0;
	void Forcefield(bool toggle);


	extern bool unlimitedstamina;
	void Unlimitedstamina(bool toggle);

	extern bool noidlekick;
	void Noidlekick(bool toggle);

	extern bool ignoreplayer;
	void Ignoreplayer(bool toggle);

	extern bool superrunbool;
	void Superrun(bool toggle);

	extern bool ultrarunbool;
	void Ultrarunbool(bool toggle);

	void changemodel(const char* model);
	void setwanted();
	void clearwanted();
	void resetped();
	void suicide();
	void maxhealth();
	void maxarmor();
	void UpdatePresence();
	inline int wantedLevel;
	inline int playeralpha = 255;
	inline int playeropacity = 255;
	inline float runspeed = 3.0;
	inline float runspeed1 = 5.0;
	void SetModifier(const char* modifier);
	void PlayAnimation(const char* name, const char* id);
	void StopAnimation();
	inline float set_wetness;
	void Set_wetness(const float& level);


	/* Weapons */
	extern bool driveitgun;
	void Driveitgun(bool toggle);

	extern bool teleportgun;
	void Teleportgun(bool toggle);

	extern bool aimbot;
	void Aimbot(bool toggle);

	extern bool deletegun;
	void Deletegun(bool toggle);

	extern bool airstrikegun;
	void Airstrikegun(bool toggle);

	extern bool exploammo;
	void Exploammo(bool toggle);

	extern bool infiniteammo;
	void Infiniteammo(bool toggle);

	extern bool deadeye;
	void Deadeye(bool toggle);

	void GiveAllWeapons();
	void RemoveAllWeapons();
	void MaxAmmo();


	/* Vehicle */
	void spawn_veh(const Hash& hash);
	void maxvehicle(const int& VehicleHandle);
	inline bool vehicle_godmode = false;
	inline bool vehicle_blip = false;
	inline bool in_vehicle = true;
	inline bool full_stats = true;

	extern bool cleanloop;
	void Cleanloop(bool toggle);

	extern bool speedbypass;
	void Speedbypass(bool toggle);

	extern bool novehkick;
	void Novehkick(bool toggle);

	extern bool fixloop;
	void Fixloop(bool toggle);

	extern bool vehgodmode;
	void Vehgodmode(bool toggle);

	extern bool hornboost;
	void Hornboost(bool toggle);

	extern bool seatbelt;
	void Seatbelt(bool toggle);

	extern bool invis_car;
	void Invis_car(bool toggle);

	extern bool flashrainbow;
	void Flashrainbow(bool toggle);

	extern bool smoothhornboost;
	void Smoothhornbbost(bool toggle);

	extern bool rainbowcar;
	void Rainbowcar(bool toggle);

	inline int VEHr = 255, VEHg = 0, VEHb = 0;
	inline int r1, g1, b1;


	inline bool hornboosteffect = false;
	inline int hornboostvalue = 50;
	inline int vehiclespeed = 100;

	inline char number_plate[10] = " ";
	void SetNumberplate(const char* text);
	void repairVehicle();
	void cleanVehicle();
	void duplicatecar();
	void colour_spam();
	void acrobatics(int i);
	void AddRamp();

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
	inline int bunker_money;
	void addMoney();
	void cayoSkipPreps();
	void cayoHardMode();
	void unlock_everything();
	void unlock_achievements();
	void max_creation_date();
	void eight_years_playtime();

	/* Online */
	extern bool esp_line_all;
	extern bool esp_box_all;
	extern bool esp_name_all;
	extern bool esp_distance_all;
	void ESP_all();

	extern bool boxesp;
	extern bool line_esp;

	extern bool npc_esp;
	void Npc_esp(bool toggle);

	void Playeresp(bool toggle, uint32_t players);

	extern bool fucktheircam;
	void Fucktheircam(bool toggle);

	extern bool spectateplayer;
	void Spectateplayer(bool toggle);

	extern bool moneynotify;
	void Moneynotify(bool toggle);

	extern bool kick_from_mk2;
	void Kick_from_mk2(bool toggle);

	extern bool freeze_player;
	void Freeze_player(bool toggle);

	extern bool ragdoll_player;
	void Ragdoll_player();

	extern bool notify_on_join;
	void Notify_on_join(bool toggle);

	extern bool crosshair;
	void Crosshair(bool toggle);

	extern bool nophone;
	void Nophone(bool toggle);

	inline bool streamer_mode = false;


	/* World*/
	inline int worldgravity = 0;
	inline float rainlevelint = 0.0;
	inline bool stoptime = false;
	inline int HoursTime = 12;
	inline int MinutesTime = 12;
	inline int clearradius = 300;

	/* Utility */
	void spawn_obj(const char* object);
	void play_particle(const char* particle);
	inline bool numpadcontrol = false;

	void boostPlayer();


	/* Misc*/
	void setOpenKey();
	void RequestControlOfEnt(Entity entity);
	void ApplyForceToEntity(Entity e, float x, float y, float z);
	void coordsDisplay(bool toggle);
	void rainbowloop();
	void Clearall();
	void Clearcars();
	void Clearpeds();
	void Clearcops();
	void nigger();
	void Spawn_companion();

	std::string Keyboard();

	extern bool rotating;
	void Rotating(bool toggle);


	void isRunning(LPCWSTR name);
	inline bool traffic_folow = false;

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
	inline bool g_spoof_username = false;
	inline bool g_spoof_ip = false;
	inline bool g_spoof_rockstar_id = false;
	inline bool leaveondetect;
	inline bool crashgame;

	extern bool rockstaradmin;
	inline bool notifyadmin;
	inline bool detectionnotify = true;
	void admindetection(bool toggle);

	extern bool shootpeds;
	void Shootpeds(bool toggle);
	

	

	inline std::string g_username = "";
	inline int g_ip_address[4] = { 109, 237, 104, 255 };
	inline uint64_t g_rockstar_id;
	inline char g_username_set[28];
	inline uint64_t rid = 0;


	void show_watermark(bool enable);
	void show_info_pool(bool enable);
	void show_fps(bool enable);
	void sessioninfo(bool toggle);

	inline bool watermark = true, pools = false, fps = true, session_info = true; 
	

	void perform();
	inline float x, y, z;
	inline float offX, offY, offZ;
}
