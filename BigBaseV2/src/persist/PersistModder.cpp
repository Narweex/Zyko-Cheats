#include "common.hpp"
#include "PersistModder.h"
#include "fiber_pool.hpp"
#include "gui/player_list.h"
#include "helpers/other.h"
#include "features.hpp"

std::string current_time()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, xorstr_("%d-%m-%Y %H-%M-%S"));
	auto str = oss.str();

	return str;
};

namespace big
{
	struct persist_ratings
	{
		float num{};
		std::string rank{};
	};

	std::vector<persist_ratings> Difficulty
	{
		{ 0, xorstr_("None") },
		{ 1.2, xorstr_("Non-Volatile") },
		{ 1.6, xorstr_("Suspicious") },
		{ 1.8, xorstr_("Modder") }
	};

	void persist_modder::save(Player player, int Level, const char* Reason)
	{
		auto json = get_detections_json(player);

		std::string difficulty = Difficulty.at(Level).rank;
		std::string time = current_time();

		json[difficulty][time][xorstr_("Reason")] = Reason;

		save_json(player, json);
	}

	void persist_modder::remove(Player player)
	{
		auto json = get_detections_json(player);

		for (auto& el : json.items())
		{
			for (const auto& rs : Difficulty)
			{
				if (rs.rank == el.key())
					json.erase(rs.rank);
			}
		}

		save_json(player, json);
	}

	void persist_modder::check_modders()
	{
		for (uint32_t i = 0; i < gta::num_players; i++)
		{
			float rating = calculate_average_rating(i);
			for (const auto& rs : Difficulty)
			{
			//	if (rating >= rs.num)
					//g_player[i].modder_level = rs.rank;
			}
		}
	}

	bool persist_modder::is_modder(Player player)
	{
		//return g_player[player].modder_level == Difficulty.at(Difficulty.size() - 1).rank;
	}

	void persist_modder::auto_notify()
	{
		for (uint32_t i = 0; i < gta::num_players; i++)
		{
			if (i == PLAYER::PLAYER_ID())
				continue;

			//if (g_player[i].modder_notified)
				continue;

			if (is_modder(i))
			{
				//g_fiber_pool->queue_job([&] { features::normal_alert(xorstr_("Detected modder"), xorstr_("High rating"), i); });

				//g_player[i].modder_notified = true;
			}
		}
	}

	float persist_modder::calculate_average_rating(Player player)
	{
		auto json = get_detections_json(player);

		float strength = 0;
		int amount = 0;
		for (auto& el : json.items())
		{
			int i = 0;
			for (const auto& rs : Difficulty)
			{
				if (rs.rank == el.key())
				{
					for (auto& el2 : json[el.key()].items())
					{
						strength += i;
						amount++;
					}
				}

				i++;
			}
		}

		float ret = strength / amount;
		if (ret < 0.f)
			ret = 0.f;

		return ret;
	}

	void persist_modder::save_json(Player player, nlohmann::json json)
	{
		auto file_path = get_player_detections(player);
		std::ofstream file(file_path, std::ios::out | std::ios::trunc);
		file << json.dump(4);
		file.close();
	}

	nlohmann::json persist_modder::get_detections_json(Player player)
	{
		auto file_path = get_player_detections(player);
		nlohmann::json locations;
		std::ifstream file(file_path);

		if (!file.fail())
			file >> locations;

		return locations;
	}

	std::filesystem::path persist_modder::get_player_detections(Player player)
	{
		std::filesystem::path file_path = get_additional_folder(xorstr_("Modders"));

		std::string converted(PLAYER::GET_PLAYER_NAME(player));
		file_path /= converted + xorstr_(".json");

		return file_path;
	}
}
