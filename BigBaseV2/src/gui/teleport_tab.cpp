#include "common.hpp"
#include "imgui_tabs.h"
#include "imgui.h"
#include "fiber_pool.hpp"
#include "features.hpp"
#include "looped/teleports.hpp"


namespace zyko
{
	void ImGuiTabs::render_teleport_tab()
	{
		ImGui::Columns(3);
		if (ImGui::CollapsingHeader(xorstr_("Airports")))
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
		if (ImGui::CollapsingHeader(xorstr_("Landmarks")))
		{
			for (auto& teleport : teleportsLandmarks)
			{
				if (ImGui::Selectable(teleport.name))
				{
					g_fiber_pool->queue_job([=] { features::teleport(teleport.x, teleport.y, teleport.z); });
				}
			}
		}
		ImGui::NextColumn();
		if (ImGui::CollapsingHeader(xorstr_("Garages")))
		{
			for (auto& teleport : teleportsGarages)
			{
				if (ImGui::Selectable(teleport.name))
				{
					g_fiber_pool->queue_job([=] { features::teleport(teleport.x, teleport.y, teleport.z); });
				}
			}
		}
		ImGui::NewLine();


		ImGui::Columns(3);
		if (ImGui::CollapsingHeader(xorstr_("IPLs")))
		{
			for (auto& teleport : teleportsIPLS)
			{
				if (ImGui::Selectable(teleport.name))
				{
					g_fiber_pool->queue_job([=] { features::teleport(teleport.x, teleport.y, teleport.z); });
				}
			}
		}
		ImGui::NextColumn();
		if (ImGui::CollapsingHeader(xorstr_("Safehouses")))
		{
			for (auto& teleport : teleportsSafehouses)
			{
				if (ImGui::Selectable(teleport.name))
				{
					g_fiber_pool->queue_job([=] { features::teleport(teleport.x, teleport.y, teleport.z); });
				}
			}
		}
		ImGui::NextColumn();
		if (ImGui::CollapsingHeader(xorstr_("Shops")))
		{
			for (auto& teleport : teleportsShops)
			{
				if (ImGui::Selectable(teleport.name))
				{
					g_fiber_pool->queue_job([=] { features::teleport(teleport.x, teleport.y, teleport.z); });
				}
			}
		}

	
	}
}