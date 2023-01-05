#include "imgui_tabs.h"
#include "features.hpp"
#include <notifications/notifications.h>

namespace zyko
{
	void ImGuiTabs::render_lobby_tab()
	{
		ImGui::BeginChild(xorstr_("##RID Converter"), ImVec2(500, 300));
		static char name1[28];
		ImGui::InputText(xorstr_("Name"), name1, sizeof(name1));
		if (ImGui::Button(xorstr_("Convert")))
		{
				
				features::rid = get_rid_from_name(name1);
				LOG(INFO) << xorstr_("You conveted RID  ") << features::rid << xorstr_("  from player ") << name1;
				std::string nigger = std::to_string(features::rid);
				Notify(name1, nigger.c_str(), 7000, Success);
				//round(3.6);
		}
		
		ImGui::EndChild();
		ImGui::Text(xorstr_(" Name Spoofer"));
		ImGui::InputText(xorstr_("Name"), features::g_username_set, sizeof(features::g_username_set));
		if (ImGui::Button(xorstr_("Set This Name")))
		{
			std::string handle(features::g_username_set);
			features::g_username = handle;
		}
		ImGui::Checkbox(xorstr_("Spoof this username"), &features::g_spoof_username);
		ImGui::EndChild();
	}
}
