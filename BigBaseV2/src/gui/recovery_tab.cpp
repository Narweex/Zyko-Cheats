#include "common.hpp"
#include "imgui_tabs.h"
#include "imgui.h"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "features.hpp"





namespace zyko
{
	void ImGuiTabs::render_recovery_tab()
	{
		ImGui::Spacing();

		ImGui::Text(xorstr_("Money"));		

		

			
			
			/*features::notify_success("Recovery", "Money Added", 3000);
			QUEUE_JOB_BEGIN_CLAUSE() {
				features::basket_transaction(-1799524201, -1018905335, 4,
					{
						{-1026265157, -1601574614, 0, 1, 1},
					});

				script::get_current()->yield(500ms);

				features::basket_transaction(-1799524201, 2108234284, 4,
					{
						{-1026265157, -1601574614, 0, 1, 1},
						{-1121615002, -1, 0, 0, 40},
						{297710065, -1121615002, 0, 40, 1},
					});

				script::get_current()->yield(500ms);

				features::basket_transaction(69656641, -886368739, 4,
					{
						{-1121615002, -1, 0, 1, -40},
						{-1573825732, -1, 0, 1, 1},
					});

				script::get_current()->yield(500ms);

				features::basket_transaction(-1799524201, -1018905335, 4,
					{
						{-1026265157, -1826424790, 0, 1, 1},
					});

				script::get_current()->yield(500ms);

				features::basket_transaction(-1799524201, -788753717, 4,
					{
						{-1026265157, -1826424790, 0, 1, 1},
						{-1573825732, -1, 0, 1, 1},
						{-1818214114, -1573825732, 2500000, 0, 1},
					});
			} QUEUE_JOB_END_CLAUSE*/
		
	/*	ImGui::Text("How to use recovery?");
		ImGui::Text("-> Dont go over 10 million in 24h");
		ImGui::Text("-> Have a bunker");
		ImGui::Text("-> Use only in public sessions");
		ImGui::Text("-> Do not overuse if you dont want to get banned");
		if (ImGui::Button(xorstr_("Add: Bunker Money"))) {
			g_fiber_pool->queue_job([=] {
				features::addMoney();
			});
		}*/

		ImGui::SameLine();

		ImGui::PushItemWidth(449.f);
		ImGui::SliderInt(xorstr_("$"), &features::bunker_money, 1, 2500000);
		ImGui::PopItemWidth();
		if (ImGui::Button("Add Startup 500k")){features::startupmoney();}
		
		ImGui::Spacing();

		ImGui::Text(xorstr_("Level"));

		ImGui::InputInt(xorstr_("Rank"), &features::level); ImGui::SameLine();
		ImGui::PopItemWidth();
		if (ImGui::Button(xorstr_("Set Rank")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				features::set_rank(features::level);
				
			}	QUEUE_JOB_END_CLAUSE
		}

		ImGui::SameLine();

		if (ImGui::Button(xorstr_("Set Crew Rank")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				features::set_crew_rank(features::level);
			}	QUEUE_JOB_END_CLAUSE
		}

		ImGui::Spacing();

		ImGui::Text(xorstr_("Unlocks"));

		if (ImGui::Button(xorstr_("Unlocks Everything")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				features::unlock_everything();
			}	QUEUE_JOB_END_CLAUSE
		}

		if (ImGui::Button(xorstr_("Extra Unlocks")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				for (int i = 0; i <= 30707; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				for (int i = 7387; i <= 7457; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				STATS::_SET_PACKED_STAT_BOOL(7466, TRUE, -1);

				for (int i = 7621; i <= 7627; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				for (int i = 15441; i <= 15446; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				for (int i = 18100; i <= 18105; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				STATS::_SET_PACKED_STAT_BOOL(15995, TRUE, -1);
				STATS::_SET_PACKED_STAT_BOOL(15548, TRUE, -1);

				for (int i = 25241; i <= 25243; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				STATS::_SET_PACKED_STAT_BOOL(25518, TRUE, -1);
				STATS::_SET_PACKED_STAT_BOOL(25519, TRUE, -1);

				for (int i = 30321; i <= 30323; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				for (int i = 31707; i <= 32283; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}

				for (int i = 32475; i <= 34123; i++) {
					STATS::_SET_PACKED_STAT_BOOL(i, TRUE, -1);
				}
			}	QUEUE_JOB_END_CLAUSE
		}

		if (ImGui::Button(xorstr_("Unlock Returning Player Content")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				*script_global(151760).as<int*>() = 2;
				*script_global(151760 + 1).as<int*>() = 2;
			} QUEUE_JOB_END_CLAUSE
		}

		if (ImGui::Button(xorstr_("Unlock All Achievements")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				features::unlock_achievements();
			}	QUEUE_JOB_END_CLAUSE
		}

		ImGui::Spacing();

		if (ImGui::Button(xorstr_("Set Char Creation Date")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				features::max_creation_date();

			}	QUEUE_JOB_END_CLAUSE
		}

		ImGui::SameLine();

		if (ImGui::Button(xorstr_("Add 8 years to the Total Playing Time")))
		{
			QUEUE_JOB_BEGIN_CLAUSE()
			{
				features::eight_years_playtime();
			}	QUEUE_JOB_END_CLAUSE
		}

		ImGui::Spacing();

		if (ImGui::TreeNode(xorstr_("Cayo Perico Helper")))
		{
			if (ImGui::Button(xorstr_("Skip Preps")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					features::cayoSkipPreps();
				}	QUEUE_JOB_END_CLAUSE
			}

			ImGui::SameLine();

			if (ImGui::Button(xorstr_("Enable Hard Mode")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					features::cayoHardMode();
				}	QUEUE_JOB_END_CLAUSE
			}

			ImGui::TreePop();
		}

		ImGui::Spacing();

		if (ImGui::TreeNode(xorstr_("Misc")))
		{
			if (ImGui::Button(xorstr_("Force Cloud Save")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					STATS::STAT_SAVE(0, 0, 3, 0);

				}	QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button(xorstr_("Max Club Popularity")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_CLUB_POPULARITY"), 1000, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);

					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_CLUB_POPULARITY"), 1000, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);

				}	QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button(xorstr_("Unlock Gender Change")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MP0_ALLOW_GENDER_CHANGE"), 52, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);

					STATS::STAT_SET_INT(RAGE_JOAAT("MP1_ALLOW_GENDER_CHANGE"), 52, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);
				}	QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button(xorstr_("Clear Total Earned/Spent Stats")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_EVC"), (uint64_t)0, TRUE);
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_TOTAL_SVC"), (uint64_t)0, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);
				}	QUEUE_JOB_END_CLAUSE
			}

			if (ImGui::Button(xorstr_("Apply Chad KD")))
			{
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_KILLS_PLAYERS"), -INT_MAX, TRUE);
					STATS::STAT_SET_INT(RAGE_JOAAT("MPPLY_DEATHS_PLAYER"), 0, TRUE);
					STATS::STAT_SAVE(0, 0, 3, 0);
				}	QUEUE_JOB_END_CLAUSE
			}
			ImGui::TreePop();
		}

		ImGui::Spacing();

		
		ImGui::Separator();

		if (ImGui::Button("Unload"))
		{
			g_running = false;
		}
	}
}