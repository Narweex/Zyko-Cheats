#pragma once
#include "UIManager.hpp"

namespace big::Lists
{
	inline const char* HeaderTypesFrontend[]
	{
		"Static",
		"Gradient"
	};

	inline UserInterface::HeaderType HeaderTypesBackend[]
	{
		UserInterface::HeaderType::Static,
		UserInterface::HeaderType::Gradient
	};

	inline std::size_t HeaderTypesPosition = 1;

	inline const char* DemoList[10]
	{
		"One",
		"Two",
		"Three",
		"Four",
		"Five",
		"Six",
		"Seven",
		"Eight",
		"Nine",
		"Ten"
	};
	inline std::vector<std::string> Import_Export
	{
		"BLAZER5", "BOXVILLE5", "DIABLOUS", "DIABLOUS2", "COMET3", "DUNE4", "DUNE5", "ELEGY", "FCR", "FCR2", "ITALIGTB", "ITALIGTB2", "NERO", "NERO2",
		 "PENETRATOR", "PHANTOM2", "RUINER2", "SPECTER", "SPECTER2", "TECHNICAL2", "TEMPESTA", "VOLTIC2", "WASTELANDER"
	};

	inline std::vector<std::string> Gunrunning
	{
		"XA21", "CADDY3", "VAGNER", "PHANTOM3", "NIGHTSHARK", "CHEETAH2", "TORERO", "HAULER2", "TECHNICAL3", "INSURGENT3", "APC", "TAMPA3", "DUNE3",
		 "TRAILERSMALL2", "HALFTRACK", "ARDENT", "OPPRESSOR"
	};

	inline std::vector<std::string> Smugglers_Run
	{
		"ALPHAZ1", "VISIONE", "STARLING", "TULA", "HAVOK", "MICROLIGHT", "MOLOTOK", "RETINUE", "ROGUE", "HOWARD", "PYRO", "CYCLONE", "RAPIDGT3",
		 "HUNTER", "MOGUL", "BOMBUSHKA", "NOKOTA", "VIGILANTE", "SEABREEZE"
	};

	inline std::vector<std::string> Doomsday_Heist
	{
		"AKULA", "BARRAGE", "CHERNOBOG", "RAIDEN", "YOSEMITE", "DELUXO", "AVENGER", "AVENGER2", "THRUSTER", "PARIAH", "STROMBERG", "COMET4", "COMET5", "RIOT2",
		 "KHANJALI", "SC1", "VOLATOL", "HERMES", "SAVESTRA", "RIATA", "STREITER", "KAMACHO", "GT500", "Z190", "VISERIS", "AUTARCH",  "NEON", "REVOLTER", "SENTINEL3", "HUSTLER"
	};

	inline std::vector<std::string> Super_Sport_Series
	{
		"CARACARA", "CHEBUREK", "DOMINATOR3", "ELLIE", "ENTITY2", "FAGALOA", "AVENGER", "FLASHGT", "GB200", "HOTRING", "ISSI3", "JESTER3", "MICHELLI",
		 "SEASPARROW", "TEZERACT", "TAIPAN", "TYRANT"
	};

	inline std::vector<std::string> After_Hours
	{
		"BLIMP3", "FREECRAWLER", "MENACER", "OPPRESSOR2", "PATRIOT2", "SCRAMJET", "STAFFORD", "STRIKEFORCE", "SWINGER", "TERBYTE"
	};

	inline std::vector<std::string> Arena_War
	{
		"BRUISER", "BRUISER2", "BRUISER3", "BRUTUS", "BRUTUS2", "BRUTUS3", "CERBERUS", "CERBERUS2", "CERBERUS3", "CLIQUE", "DEATHBIKE", "DEATHBIKE2", "DEATHBIKE3",
		 "DEVESTE", "DEVIANT", "DOMINATOR4", "DOMINATOR5", "DOMINATOR6", "IMPALER", "IMPALER2", "IMPALER3", "IMPALER4", "IMPERATOR", "IMPERATOR2", "IMPERATOR3", "ISSI4", "ISSI5", "ISSI6",
		 "ITALIGTO", "MONSTER3", "MONSTER4", "MONSTER5", "RCBANDITO", "SCARAB", "SCARAB2", "SCARAB3", "SCHLAGEN", "SLAMVAN4", "SLAMVAN5", "SLAMVAN6", "TULIP", "VAMOS",
		 "ZR380", "ZR3802", "ZR3803"
	};

	inline std::vector<std::string> Diamond_Casino
	{
		"CARACARA2", "DYNASTY", "EMERUS",
		"GAUNTLET3", "GAUNTLET4", "HELLION", "ISSI7",
		"JUGULAR", "KRIEGER", "LOCUST", "NEBULA",
		"NEO", "NOVAK", "PARAGON", "PARAGON2",
		"PEYOTE2", "RROCKET", "S80",
		"THRAX", "ZION3", "ZORRUSSO"
	};

	inline std::vector<std::string> Diamond_Casino_Heist
	{
		"FORMULA", "FORMULA2", "FURIA", "IMORGON",
		"KOMODA", "REBLA", "STRYDER", "VSTR",
		"ASBO", "EVERON", "KANJO", "OUTLAW",
		"RETINUE2", "SUGOI", "SULTAN2",
		"VAGRANT", "YOSEMITE2", "JB7002",
		"MINITANK", "ZHABA"
	};

	inline std::vector<std::string> Perico_heist
	{
		"annihilator2", "seasparrow2","seasparrow3", "vetir","weevil", "brioso2",
		"patrolboat", "longfin", "kosatka", "dinghy5", "avisa", "slamtruck", "toreador", "veto2",
		"veto", "italirsx", "squaddie", "alkonost", "winky", "verus", "manchez2",
	};

	inline std::vector<std::string> lsc_tunners
	{
		"calico",
		"comet6",
		"cypher",
		"dominator7",
		"dominator8",
		"euros",
		"freightcar2",
		"futo2",
		"growler",
		"jester4",
		"previon",
		"remus",
		"rt3000",
		"sultan3",
		"tailgater2",
		"vectre",
		"warrener",
		"warrener2",
		"zr350"
	};

	inline std::vector<std::string> the_contract
	{
		"buffalo4",
		"granger2",
		"iwagen",
		"patriot3",
		"astron",
		"baller7",
		"champion",
		"cinquemila",
		"comet7",
		"deity",
		"ignus",
		"jubilee",
		"reever",
		"shinobi",
		"zeno"
	};

	inline std::vector<std::string> Boats1
	{
		"DINGHY", "DINGHY2", "DINGHY3", "JETMAX",
		"MARQUIS", "PREDATOR", "SEASHARK", "SEASHARK2", "SEASHARK3",
		"SPEEDER", "SPEEDER2", "SQUALO", "SUBMERSIBLE", "SUBMERSIBLE2",
		"SUNTRAP", "TORO", "TORO2", "TROPIC", "TROPIC2",
		"TUG"
	};

	inline std::vector<std::string> Commercial1
	{
		"BENSON", "BIFF", "HAULER", "HAULER2", "MULE", "MULE2",
		"MULE3", "MULE4", "PACKER", "PHANTOM","PHANTOM2", "PHANTOM3", "POUNDER", "POUNDER2", "STOCKADE",
		"STOCKADE3", "TERBYTE", "CERBERUS", "CERBERUS2", "CERBERUS3",
	};

	inline std::vector<std::string> Compacts1
	{
		"BLISTA", "BLISTA2", "BLISTA3", "BRIOSO", "DILETTANTE", "DILETTANTE2",
		"PANTO", "PRAIRIE", "RHAPSODY", "ISSI2", "ISSI3", "ISSI4", "ISSI5", "ISSI6", "ISSI7"
	};

	inline std::vector<std::string> Coupes1
	{
		"COGCABRIO", "EXEMPLAR", "F620", "FELON", "FELON2",
		"JACKAL", "ORACLE", "ORACLE2", "SENTINEL", "SENTINEL2",
		"WINDSOR", "WINDSOR2", "ZION", "ZION2"
	};

	inline std::vector<std::string> Cycles1
	{
		"BMX", "CRUISER", "FIXTER", "SCORCHER", "TRIBIKE",
		"TRIBIKE2", "TRIBIKE3"
	};

	inline std::vector<std::string> Emergency1
	{
		"AMBULANCE", "FBI", "FBI2", "FIRETRUK",
		"POLICE", "POLICE2", "POLICE3",
		"POLICE4", "POLICEOLD1", "POLICEOLD2", "POLICET", "POLICEB",
		"PBUS", "PRANGER", "LGUARD",
		"SHERIFF", "SHERIFF2", "RIOT", "RIOT2"
	};

	inline std::vector<std::string> Helicopters1
	{
		"ANNIHILATOR", "BUZZARD", "BUZZARD2",
		"CARGOBOB", "CARGOBOB2", "CARGOBOB3", "CARGOBOB4", "FROGGER",
		"FROGGER2", "MAVERICK", "POLMAV", "SAVAGE", "SKYLIFT",
		"SUPERVOLITO", "SUPERVOLITO2", "SWIFT", "SWIFT2", "VALKYRIE",
		"VALKYRIE2", "VOLATUS", "HAVOK", "MICROLIGHT", "HUNTER", "AKULA",
		"AVENGER", "AVENGER2", "SEASPARROW"
	};

	inline std::vector<std::string> Industrial1
	{
		"BULLDOZER", "CUTTER", "DUMP", "FLATBED", "GUARDIAN",
		"HANDLER", "MIXER", "MIXER2", "RUBBLE", "TIPTRUCK",
		"TIPTRUCK2"
	};

	inline std::vector<std::string> Military1
	{
		"BARRACKS", "BARRACKS2", "CRUSADER", "RHINO",  "APC",
		"HALFTRACK", "BARRAGE", "CHERNOBOG", "KHANJALI", "SCARAB", "SCARAB2", "SCARAB3",
		"THRUSTER"
	};

	inline std::vector<std::string> Motorcycles1
	{
		"AKUMA", "AVARUS", "BAGGER", "BATI", "BATI2",
		"BF400", "CARBONRS", "CHIMERA", "CLIFFHANGER", "DAEMON",
		"DAEMON2", "DEFILER", "DOUBLE", "ENDURO", "ESSKEY",
		"FAGGIO", "FAGGIO2", "FAGGIO3", "GARGOYLE", "HAKUCHOU",
		"HAKUCHOU2", "HEXER", "INNOVATION", "LECTRO", "MANCHEZ",
		"NEMESIS", "NIGHTBLADE", "PCJ", "RATBIKE", "RUFFIAN",
		"SANCHEZ", "SANCHEZ2", "SANCTUS", "SHOTARO", "SOVEREIGN",
		"THRUST", "VADER", "VINDICATOR", "VORTEX", "WOLFSBANE",
		"ZOMBIEA", "ZOMBIEB", "DIABLOUS", "DIABLOUS2", "FCR",
		"FCR2", "OPPRESSOR", "OPPRESSOR2", "DEATHBIKE", "DEATHBIKE2", "DEATHBIKE3", "RROCKET"

	};
	inline std::vector<std::string> Muscle1
	{
		"BLADE", "BUCCANEER", "BUCCANEER2", "CHINO", "CHINO2",
		"COQUETTE3", "DOMINATOR", "DOMINATOR2", "DOMINATOR3", "DOMINATOR4", "DOMINATOR5", "DOMINATOR6", "DUKES", "DUKES2",
		"GAUNTLET", "GAUNTLET2", "FACTION", "FACTION2", "FACTION3",
		"HOTKNIFE", "LURCHER", "MOONBEAM", "MOONBEAM2", "NIGHTSHADE",
		"PHOENIX", "PICADOR", "RATLOADER", "RATLOADER2", "RUINER", "RUINER2", "RUINER3",
		"SABREGT", "SABREGT2", "SLAMVAN", "SLAMVAN2", "SLAMVAN3",
		"STALION", "STALION2", "TAMPA", "TAMPA3", "VIGERO", "VIRGO",
		"VIRGO2", "VIRGO3", "VOODOO", "VOODOO2", "ELLIE",
		"CLIQUE", "DEVIANT", "IMPALER", "IMPALER2", "IMPALER3", "IMPALER4",
		"IMPERATOR", "IMPERATOR2", "IMPERATOR3", "SLAMVAN4", "SLAMVAN5", "SLAMVAN6",
		"TULIP", "VAMOS","GAUNTLET3", "GAUNTLET4", "YOSEMITE"
	};

	inline std::vector<std::string> OffRoad1
	{
		"BFINJECTION", "BIFTA", "BLAZER", "BLAZER2", "BLAZER3",
		"BLAZER4", "BLAZER5", "BODHI2", "BRAWLER", "DLOADER", "DUBSTA3",
		"DUNE", "DUNE2", "DUNE3", "DUNE4", "DUNE5", "INSURGENT", "INSURGENT2", "INSURGENT3", "KALAHARI",
		"MARSHALL", "MESA3", "MONSTER", "MONSTER3", "MONSTER4", "MONSTER5", "RANCHERXL", "RANCHERXL2",
		"REBEL", "REBEL2", "SANDKING", "SANDKING2", "TECHNICAL", "TECHNICAL2", "TECHNICAL3", "NIGHTSHARK",
		"TROPHYTRUCK", "TROPHYTRUCK2",
		"COMET4", "RIATA", "KAMACHO", "CARACARA","CARACARA2", "FREECRAWLER", "MENACER",
		"BRUISER", "BRUISER2", "BRUISER3", "BRUTUS", "BRUTUS2", "BRUTUS3", "RCBANDITO", "HELLION"

	};

	inline std::vector<std::string> Planes1
	{
		"BESRA", "BLIMP", "BLIMP2", "BLIMP3", "CARGOPLANE", "CUBAN800", "DODO", "DUSTER",
		"HYDRA", "JET", "LAZER", "LUXOR", "LUXOR2",
		"MAMMATUS", "MILJET", "NIMBUS", "SHAMAL", "STUNT",
		"TITAN", "VELUM", "VELUM2", "VESTRA", "ALPHAZ1",
		"STARLING", "TULA", "MOLOTOK", "ROGUE", "HOWARD", "PYRO",
		"MOGUL", "BOMBUSHKA", "NOKOTA", "SEABREEZE", "VOLATOL",
		"STRIKEFORCE"
	};

	inline std::vector<std::string> Sedans1
	{
		"ASEA", "ASEA2", "ASTEROPE", "COG55", "COG552",
		"COGNOSCENTI", "COGNOSCENTI2", "EMPEROR", "EMPEROR2", "EMPEROR3",
		"FUGITIVE", "GLENDALE", "INGOT", "INTRUDER", "LIMO2",
		"PREMIER", "PRIMO", "PRIMO2", "REGINA", "ROMERO",
		"SCHAFTER2", "SCHAFTER5", "SCHAFTER6", "STANIER", "STRATUM",
		"STRETCH", "SUPERD", "SURGE", "TAILGATER", "WARRENER",
		"WASHINGTON", "REVOLTER", "STAFFORD"
	};

	inline std::vector<std::string> Service1
	{
		"AIRBUS", "BRICKADE", "BUS", "PBUS2", "COACH", "RALLYTRUCK",
		"RENTALBUS", "TAXI", "TOURBUS", "TRASH", "TRASH2",
		"WASTELANDER"
	};

	inline std::vector<std::string> Sports1
	{
		"ALPHA", "BANSHEE", "BESTIAGTS",
		"BUFFALO", "BUFFALO2", "BUFFALO3", "CARBONIZZARE", "COMET2",  "COMET3", "COMET5",
		"COQUETTE", "ELEGY", "ELEGY2", "FELTZER2", "FUROREGT", "FUSILADE",
		"FUTO", "JESTER", "JESTER2", "JESTER3", "KHAMELION", "KURUMA",
		"KURUMA2", "LYNX", "MASSACRO", "MASSACRO2", "NINEF",
		"NINEF2", "OMNIS", "PENUMBRA", "RAPIDGT", "RAPIDGT2", "RAPIDGT3",
		"RAPTOR", "SCHAFTER3", "SCHAFTER4", "SCHWARZER", "SEVEN70", "STREITER",
		"SULTAN", "SURANO", "SPECTER", "SPECTER2", "TAMPA2", "TROPOS", "VERLIERER2",
		"RUSTON", "RAIDEN", "PARIAH",  "NEON", "FLASHGT", "GB200",
		"HOTRING", "SCHLAGEN", "ZR380", "ZR3802", "ZR3803", "JUGULAR", "LOCUST", "NEBULA",
		"PARAGON", "PARAGON2"
	};

	inline std::vector<std::string> SportsClassics1
	{
		"BTYPE", "BTYPE2", "BTYPE3", "CASCO", "COQUETTE2",
		"FELTZER3", "JB700", "MAMBA", "MANANA", "MONROE",
		"PEYOTE", "PEYOTE2", "PIGALLE", "STINGER", "STINGERGT", "CHEETAH2", "TORERO", "TORNADO",
		"TORNADO2", "TORNADO3", "TORNADO4", "TORNADO5", "TORNADO6", "ZION3", "RETINUE", "SAVESTRA",
		"ZTYPE", "INFERNUS2", "TURISMO2", "DELUXO", "STROMBERG", "ARDENT",
		"HERMES", "GT500", "Z190", "VISERIS", "SENTINEL3", "HUSTLER", "CHEBUREK",
		"MICHELLI", "FAGALOA", "SWINGER", "DYNASTY"

	};

	inline std::vector<std::string> Super1
	{
		"ADDER", "BANSHEE2", "BULLET", "CHEETAH", "ENTITYXF",
		"FMJ", "SHEAVA", "INFERNUS", "NERO", "NERO2","OSIRIS", "LE7B",
		"ITALIGTB", "ITALIGTB2", "PFISTER811", "PROTOTIPO", "REAPER", "SULTANRS", "T20",
		"TEMPESTA", "TURISMOR", "TYRUS", "VACCA", "VOLTIC", "VOLTIC2", "ZENTORNO", "PENETRATOR", "GP1",
		"XA21", "VAGNER", "VIGILANTE", "VISIONE", "CYCLONE", "SC1", "AUTARCH", "ENTITY2",
		"TEZERACT", "TAIPAN", "TYRANT", "SCRAMJET", "DEVESTE", "ITALIGTO", "EMERUS", "KRIEGER",
		 "S80","THRAX", "ZORRUSSO"
	};

	inline std::vector<std::string> SUVs1
	{
		"BALLER", "BALLER2", "BALLER3", "BALLER4", "BALLER5",
		"BALLER6", "BJXL", "CAVALCADE", "CAVALCADE2", "CONTENDER",
		"DUBSTA", "DUBSTA2", "FQ2", "GRANGER", "GRESLEY",
		"HABANERO", "HUNTLEY", "LANDSTALKER", "MESA", "MESA2",
		"PATRIOT", "PATRIOT2", "RADI", "ROCOTO", "SEMINOLE", "SERRANO",
		"XLS", "XLS2", "NOVAK"
	};

	inline std::vector<std::string> Trailer1
	{
		"ARMYTANKER", "ARMYTRAILER", "ARMYTRAILER2", "BALETRAILER", "BOATTRAILER",
		"CABLECAR","DOCKTRAILER", "GRAINTRAILER", "PROPTRAILER", "RAKETRAILER",
		"TR2", "TR3", "TR4", "TRFLAT", "TVTrailer", "Tanker", "Tanker2", "TrailerLogs",
		"TrailerSmall", "TRAILERSMALL2", "Trailers", "Trailers2", "Trailers3"
	};

	inline std::vector<std::string> Trains1
	{
		"FREIGHT", "FREIGHTCAR", "FREIGHTCONT1", "FREIGHTCONT2",
		"FREIGHTGRAIN", "METROTRAIN", "TANKERCAR"
	};

	inline std::vector<std::string> Utility1
	{
		"AIRTUG", "CADDY", "CADDY2", "CADDY3", "DOCKTUG", "FORKLIFT",
		"MOWER", "RIPLEY", "SADLER", "SADLER2", "SCRAP",
		"TOWTRUCK", "TOWTRUCK2", "TRACTOR", "TRACTOR2", "TRACTOR3",
		"UTILLITRUCK", "UTILLITRUCK2", "UTILLITRUCK3"
	};

	inline std::vector<std::string> Vans1
	{
		"BISON", "BISON2", "BISON3", "BOBCATXL", "BOXVILLE",
		"BOXVILLE2", "BOXVILLE3", "BOXVILLE4", "BURRITO", "BURRITO2",
		"BURRITO3", "BURRITO4", "BURRITO5", "CAMPER", "GBURRITO",
		"GBURRITO2", "JOURNEY", "MINIVAN", "MINIVAN2", "PARADISE",
		"PONY", "PONY2", "RUMPO", "RUMPO2", "RUMPO3",
		"SPEEDO", "SPEEDO2", "SPEEDO4", "SURFER", "SURFER2", "TACO",
		"YOUGA", "YOUGA2"
	};

	inline std::size_t DemoListPos = 0;
}
