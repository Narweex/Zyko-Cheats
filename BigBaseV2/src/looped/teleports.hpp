#include "../common.hpp"
#include "features.hpp"

namespace big
{
	struct TeleportAirports
	{
		const char* name;
		float x, y, z;
		
	};
	inline std::vector<TeleportAirports> teleportsAirports
	{
		{"LS International", -1102.2910f, -2894.5160f, 13.9467f},
		{"Sandy Shores Airport", 1741.4960f, 3269.2570f, 41.6014f},
		{"Grapeseed Airport", 2137.5f, 4809.4f, 41.1181f}
	};

	struct TeleportIPLS
	{
		const char* name;
		float x, y, z;

	};
	inline std::vector<TeleportIPLS> teleportsIPLS
	{
		{"Maze Bank", -74.94243f, -818.63446f, 326.174}
	};

	struct TeleportLandmarks
	{
		const char* name;
		float x, y, z;

	};
	inline std::vector<TeleportLandmarks> teleportsLandmarks
	{
		{"Maze Bank", -74.94243f, -818.63446f, 326.174}
	};

	struct TeleportGarages
	{
		const char* name;
		float x, y, z;

	};
	inline std::vector<TeleportGarages> teleportsGarages
	{
		{"Maze Bank", -74.94243f, -818.63446f, 326.174}
	};

	struct TeleportSafehouses
	{
		const char* name;
		float x, y, z;

	};
	inline std::vector<TeleportSafehouses> teleportsSafehouses
	{
		{"Maze Bank", -74.94243f, -818.63446f, 326.174}
	};

	struct TeleportShops
	{
		const char* name;
		float x, y, z;

	};
	inline std::vector<TeleportShops> teleportsShops
	{
		{"Maze Bank", -74.94243f, -818.63446f, 326.174}
	};
	
}
