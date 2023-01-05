#include "common.hpp"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "features.hpp"
#include "script_global.hpp"
#include "imgui_tabs.h"
#include <gui/player_list.h>


namespace zyko
{


	void ImGuiTabs::render_players_tab()
	{

		if (!*g_pointers->m_is_session_started)
		{
			
			if (ImGui::ListBoxHeader(xorstr_("##playerlist"), ImVec2(200, -1)))
			{
				for (auto& pair : playerlist)
				{
					if (pair.second.name != nullptr)
					{
						if (ImGui::Selectable(pair.second.name, pair.second.id == features::g_selected_player))
						{
							if (features::g_selected_player != pair.second.id)
								features::g_selected_player = pair.second.id;
						}
					}
				}
				ImGui::ListBoxFooter();
			}

				


			ImGui::SameLine(200.f);

			ImGui::BeginChild(xorstr_("##playerfeatures"), ImVec2(425.f, 520.f));
			{

				//if (ImGui::CollapsingHeader(xorstr_("Player Info")))
				//{
				//	ImGui::Columns(2);

				//	ImGui::BeginGroup();
				//	//ImGui::Text(xorstr_("Name: %lld"), g_player_list.name);
				//	ImGui::Text(xorstr_("Player ID: %lld"), features::g_selected_player);
				//	ImGui::Text(xorstr_("Is Modder: ")); ImGui::SameLine();
				//	ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(180, 0, 0, 255)), g_player_list.is_modder ? "Yes" : "No");
				//	ImGui::Text("Coords X: %d.%d.%d", g_player_list.coordsx, g_player_list.coordsy, g_player_list.coordsz);

				//	ImGui::Text(xorstr_("In Vehicle: %s"), g_player_list.is_vehicle ? "Yes" : "No");
				//	ImGui::Text(xorstr_("Player Speed: %i MPH"), g_player_list.speed);



				//	ImGui::EndGroup();


				//	ImGui::BeginGroup();
				//	ImGui::Text(xorstr_("Is Alive: ")); ImGui::SameLine();
				//	if (g_player_list.health < 1)
				//		ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(0, 180, 0, 255)), xorstr_("No"));
				//	else
				//		ImGui::Text(xorstr_("Yes"));
				//	ImGui::NewLine();

				//	ImGui::Text(xorstr_("Health: %f"), g_player_list.health);
				//	ImGui::Text(xorstr_("Armour: %f"), g_player_list.armour);






					ImGui::EndGroup();

					//ImGui::Columns(1);
					ImGui::Separator();
				//}
			}
			ImGui::EndChild();
		}
		else
			ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 255)), xorstr_("Connect to GTA Online"));

	}
}
