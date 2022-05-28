#include "common.hpp"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "features.hpp"
#include "script_global.hpp"
#include "players_tab.h"
#include "player_list.h"

namespace big
{
	void players_tab::render_players_tab()
	{
		if (ImGui::BeginTabItem(xorstr_("Players")))
		{
			if (ImGui::ListBoxHeader(xorstr_("##playerlist"), ImVec2(200, -1)))
			{
				for (auto pair : playerlist)
				{
					if (pair.second.name != nullptr)
					{
						if (ImGui::Selectable(pair.second.name, pair.second.id == features::g_selected_player))
						{
							if (features::g_selected_player != pair.second.id)
								features::g_selected_player = pair.second.id;
						}
					}
				}
				ImGui::ListBoxFooter();
			}
			ImGui::EndTabItem();
		}

		ImGui::SameLine(215.f);

		ImGui::BeginChild(xorstr_("##playerfeatures"), ImVec2(925.f, -1)); 
		{
			for (auto pair : playerlist)
			{
				if (pair.second.name != nullptr)
				{
					if (ImGui::CollapsingHeader(xorstr_("Player Info")))
					{
						ImGui::Columns(2);

						ImGui::BeginGroup();
						ImGui::Text(xorstr_("Player ID: %lld"), features::g_selected_player);

						ImGui::EndGroup();

						ImGui::NextColumn();

						ImGui::BeginGroup();
						ImGui::Text(xorstr_("Health: %d"), pair.second.health);


						ImGui::EndGroup();

						ImGui::Columns(1);
						ImGui::Separator();
					}
				}
			}

			if (ImGui::Button(xorstr_("Teleport"))) {
				QUEUE_JOB_BEGIN_CLAUSE()
				{
					Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(features::g_selected_player), false);
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), coords.x, coords.y, coords.z, 0, 0, 1);
				} QUEUE_JOB_END_CLAUSE
			} 
				
		}
		ImGui::EndChild();
	} 
}