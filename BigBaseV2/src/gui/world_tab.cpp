#include "common.hpp"
#include "imgui_tabs.h"
#include "features.hpp"
#include "fiber_pool.hpp"
#include <looped/gtaData.hpp>


namespace zyko
{
	void ImGuiTabs::render_world_tab()
	{

		ImGui::BeginChild(xorstr_("##cleararea"), ImVec2(300, 280), true);
		ImGui::Text(xorstr_("Clear area"));
		ImGui::Separator();


		ImGui::SliderInt("Radius", &features::clearradius, 0, 300);
		if (ImGui::Button(xorstr_("Clear from everything"))) {
			g_fiber_pool->queue_job([=] {
				features::Clearall();
				});
		}
		if (ImGui::Button(xorstr_("Clear from vehicles"))) {
			g_fiber_pool->queue_job([=] {
				features::Clearcars();				
				});
		}
		if (ImGui::Button(xorstr_("Clear from peds"))) {
			g_fiber_pool->queue_job([=] {
			features::Clearpeds();
				});
		}
		if (ImGui::Button(xorstr_("Clear from cops"))) {
			g_fiber_pool->queue_job([=] {
			features::Clearcars();
				});
		}


		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::PushItemWidth(200.f);
		
		if (ImGui::BeginCombo(xorstr_(""), "Weather Types"))
		{
			for (auto& weathertypes : WeatherTypes)
			{
				if (ImGui::Selectable(weathertypes))
				{
					g_fiber_pool->queue_job([=] {	features::changeweather(weathertypes); });
				}

			}
			ImGui::EndCombo();
		}
	}
}