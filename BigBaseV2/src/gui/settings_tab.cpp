#include "imgui_tabs.h"
#include "features.hpp"
#include <notifications/notifications.h>
#include "style.hpp"

namespace zyko
{
	void ImGuiTabs::render_settings_tab()
	{

		if (ImGui::IsItemHovered((ImGui::Checkbox(xorstr_("Streamer Mode"), &features::streamer_mode))))
		{
			ImGui::BeginTooltip();
			ImGui::Text(xorstr_("Hides Players IPs"));
			ImGui::EndTooltip();
		}

		ImGui::Spacing();

		if (ImGui::Button(xorstr_("unload"))) {
			g_running = false;
		}

		ImGui::Spacing();

		if (ImGui::Button(xorstr_("List Mode")))
		{
			g_list = true;
			g_UiManager->m_Opened = true;
		}

		ImGui::Spacing();

		if (ImGui::Button(xorstr_("Close game")))
		{
			exit(0);
		}

		ImGui::Checkbox(xorstr_("Display FPS"), &features::fps);
		ImGui::Checkbox(xorstr_("Display Watermark"), &features::watermark);
		ImGui::Checkbox(xorstr_("Display Session Info"), &features::session_info);

		ImGui::SliderInt("Menu Background", &customization.background_colour, 0, 255);

		if (ImGui::Button("Set Style"))
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg] = ImColor(19, 14, 24, customization.background_colour);
		}

	}
}