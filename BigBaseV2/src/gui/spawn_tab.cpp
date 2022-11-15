#include "common.hpp"
#include "imgui_tabs.h"
#include "imgui.h"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "features.hpp"
#include "gui/list/Lists.hpp"


namespace zyko
{
	void ImGuiTabs::render_spawn_tab()
	{
		if (ImGui::TreeNode(xorstr_("Super")))
		{
			/*for (auto& super : Lists::Super1)
			{
				if (ImGui::Button(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(super)))))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(super)); });
				}
			}*/
		}
	}
}