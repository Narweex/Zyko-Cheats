#pragma once

static const char* TimecycleModifiers[]
{
	"underwater",
	"underwater_deep",
	"NoAmbientmult",
	"superDARK",
	"Bank_HLWD",
	"Barry1_Stoned",
	"BarryFadeOut",
	"BloomLight",
	"buildingTOP",
	"BulletTimeDark",
	"BulletTimeLight",
	"CAMERA_BW",
	"CAMERA_secuirity",
	"CAMERA_secuirity_FUZZ",
	"canyon_mission",
	"CHOP",
	"cinema",
	"cinema_001",
	"whitenightlighting",
	"WhiteOut",
	"Tunnel",
	"drug_drive_blend01",
	"drug_drive_blend02",
	"drug_flying_01",
	"RaceTurboDark",
	"RaceTurboFlash",
	"RaceTurboLight",
	"prologue_reflection_opt",
	"prologue_shootout",
	"Prologue_shootout_opt",
	"prologue",
	"pulse"
};

static const char* PedModels[]
{
	"A_C_Pigeon",
	"a_m_m_acult_01",
	"u_m_y_juggernaut_01",
};

static const struct Animations {
	const char* name;
	const char* id;
};

inline std::vector<Animations> animations
{
	{"timetable@lamar@ig_2", "grill_protien_like_clockwork_lamar"},
	{"rcmpaparazzo_2", "shag_loop_poppy"}
};