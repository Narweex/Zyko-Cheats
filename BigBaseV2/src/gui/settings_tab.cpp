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
			
			Auth();

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
		if (ImGui::IsItemHovered((ImGui::Checkbox(xorstr_("Discord RPC"), &features::discord_rpc))))
		{
			ImGui::BeginTooltip();
			ImGui::Text(xorstr_("Users in discord will see you are using Zyko"));
			ImGui::EndTooltip();
		}

		ImGui::SameLine();
		if (ImGui::ListBoxHeader("##ThemeChanger", ImVec2(200, -1)))
		{
			ImGuiStyle& custom = ImGui::GetStyle();
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColorName(i);
				ImGui::ColorEdit4((std::string("##") + name).c_str(), (float*)&custom.Colors[i], ImGuiColorEditFlags_NoInputs);
				ImGui::SameLine(); ImGui::Text(name);
			}
			ImGui::ListBoxFooter();
		}

	}
}