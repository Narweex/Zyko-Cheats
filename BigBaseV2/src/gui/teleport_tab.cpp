#include "common.hpp"
#include "teleport_tab.h"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "gui/list/UIManager.hpp"
#include "features.hpp"
#include "script_global.hpp"
#include "looped/teleports.hpp"

namespace big
{
	void teleport_tab::render_teleport_tab()
	{
		ImGui::Columns(3);
		if (ImGui::CollapsingHeader("Airports"))
		{
			for (auto& teleport : teleportsAirports)
			{
				if (ImGui::Selectable(teleport.name))
				{
					g_fiber_pool->queue_job([=] { features::teleport(teleport.x, teleport.y, teleport.z); });
				}
			}
		}
		ImGui::NextColumn();

	
	}
}