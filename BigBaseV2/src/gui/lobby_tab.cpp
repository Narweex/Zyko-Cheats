#include "lobby_tab.h"
#include "features.hpp"
#include <imgui.h>
#include <helpers/other.h>
namespace big
{
	void lobby_tab::render_lobby_tab()
	{
		static char name1[28];
		ImGui::InputText("Name", name1, sizeof(name1));
		if (ImGui::Button("Convert"))
		{
				std::string name2(name1);
				features::rid = get_rid_from_name(name2);
				LOG(INFO) << "You conveeted RID " << features::rid << "from player " << name1;	
		}

		//std::string rid = std::to_string(features::rid);
		//char rid2[28] = char(features::rid);
		//ImGui::Text(rid2);
		ImGui::BeginChild("##options1", ImVec2(150, 140), true);

		ImGui::Text(xorstr_(" Name Spoofer"));
		//ImGui::InputText("Name", &features::g_username, sizeof(name1));
		ImGui::Checkbox("Spoof this username", &features::g_spoof_username);
		ImGui::EndChild();

	}
}
