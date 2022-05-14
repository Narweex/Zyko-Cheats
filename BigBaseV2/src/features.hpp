#pragma once
#include "common.hpp"

namespace big::features
{
	void run_tick();
	void script_func();
	void on_present();

	inline ULONGLONG ticks[] = { 0, 0, 0, 0 };
	inline ULONGLONG tick_conf[] = { 50, 250, 2000, 25003 };
	inline int g_current_tick_job{};

	/* Self */
	inline bool godmode = false;
	inline bool neverWanted = false;
	inline bool sessioninfo = false;
	void set_rank(int rpvalue);
	inline int level{};
	void set_crew_rank(int rpvalue);
	void teleport(int x, int y, int z);
	void spawn_obj(const char* object);
	inline bool deletegun = false;
	inline bool offradar = false;
	inline bool fixloop = false;
	inline bool cleanloop = false;
	inline bool speedbypass = true;
	inline bool freecam = false;;
	inline int crew_level{};
	inline int SessionInfoX = 0.020f;
	inline int SessionInfoText = 0.4f;
	inline bool speedometer = true;
	inline bool loadingtext = true;
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

	/*Vehicle spawner*/
	void spawn_veh(Hash hash);
	inline bool vehicle_godmode = false;
	inline bool vehicle_blip = false;
	inline bool in_vehicle = true;
	inline bool full_stats = true;

	/* Online */
	inline std::uint32_t g_selected_player{};

	void basket_transaction(int cat, int action, int flag, std::vector<std::array<int, 5>> items);
}
