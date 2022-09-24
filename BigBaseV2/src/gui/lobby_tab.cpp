#include "lobby_tab.h"
#include "features.hpp"

namespace big
{
	void lobby_tab::render_lobby_tab()
	{
		ImGui::BeginChild("##RID Converter", ImVec2(-1, 500));
		static char name1[28];
		ImGui::InputText("Name", name1, sizeof(name1));
		if (ImGui::Button("Convert"))
		{
				std::string name2(name1);
				features::rid = get_rid_from_name(name2);
				LOG(INFO) << "You conveted RID  " << features::rid << "  from player " << name1;
				
		}
		//ImGui::Text(&features::rid);
		
		ImGui::EndChild();
		ImGui::Text(xorstr_(" Name Spoofer"));
		ImGui::InputText("Name", features::g_username_set, sizeof(features::g_username_set));
		if (ImGui::Button("Set This Name"))
		{
			std::string handle(features::g_username_set);
			features::g_username = handle;
		}
		ImGui::Checkbox("Spoof this username", &features::g_spoof_username);
		ImGui::EndChild();
	}
}
