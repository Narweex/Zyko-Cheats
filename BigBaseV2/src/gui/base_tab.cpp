#include "common.hpp"
#include "base_tab.h"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "gui/list/UIManager.hpp"

namespace big
{
	void base_tab::render_base_tab()
	{
		if (ImGui::BeginTabItem("Test"))
		{
			if (ImGui::Button(xorstr_("Clear Wanted Level"))) {
				QUEUE_JOB_BEGIN_CLAUSE() {
					if (auto player_info = g_local_player->m_player_info) {
						if (player_info->m_is_wanted)
							player_info->m_wanted_level = 0;
					}
				} QUEUE_JOB_END_CLAUSE
			}
			ImGui::SameLine();
			if (ImGui::Button(xorstr_("Add Wanted Level"))) {
				QUEUE_JOB_BEGIN_CLAUSE() {
					if (auto player_info = g_local_player->m_player_info) {
							player_info->m_wanted_level = 5;
					}
				} QUEUE_JOB_END_CLAUSE
			}
			if (ImGui::Button("List Mode"))
			{
				//g_list = true;
				g_UiManager->m_Opened = true;
			}
			ImGui::Separator();

			if (ImGui::Button("Unload"))
			{
				g_running = false;
			}
			ImGui::EndTabItem();
		}
	}
}