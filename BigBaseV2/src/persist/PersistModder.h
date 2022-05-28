#pragma once
#include "natives.hpp"

namespace big
{
	class persist_modder
	{
	public:
		static bool is_modder(Player player);
		static void check_modders();
		static void auto_notify();
		static void save(Player player, int Level, const char* Reason);
		static void remove(Player player);
		static float calculate_average_rating(Player player);
	private:
		static void save_json(Player player, nlohmann::json json);
		static nlohmann::json get_detections_json(Player player);
		static std::filesystem::path get_player_detections(Player player);
	};
}