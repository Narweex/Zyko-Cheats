#include "common.hpp"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "features.hpp"
#include "script_global.hpp"
#include "players_tab.h"
#include "player_list.h"

namespace big
{
	
	
	void players_tab::render_players_tab()
	{
		if (ImGui::BeginTabItem(xorstr_("Players")))
		{
			if (*g_pointers->m_is_session_started)
			{
				if (ImGui::ListBoxHeader(xorstr_("##playerlist"), ImVec2(200, -1)))
				{
					for (auto pair : playerlist)
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


				ImGui::SameLine(215.f);

				ImGui::BeginChild(xorstr_("##playerfeatures"), ImVec2(925.f, -1));
				{
					if (ImGui::CollapsingHeader(xorstr_("Player Info")))
					{
						ImGui::Columns(2);

						ImGui::BeginGroup();
						ImGui::Text(xorstr_("Name: %lld"), g_player_list.name);
						ImGui::Text(xorstr_("Player ID: %lld"), features::g_selected_player);
						ImGui::Text(xorstr_("Is Modder: ")); ImGui::SameLine(); 
						if (g_player_list.is_modder) 
							ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(180, 0, 0, 255)), xorstr_("Yes")); 
						else
							ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(0, 180, 0, 255)), xorstr_("No"));

						//ImGui::Text(xorstr_("Wanted Level: %i / 5"), g_player_list.wanted);
						ImGui::Text(xorstr_("In Vehicle: %s"), g_player_list.is_vehicle ? "Yes" : "No");
					//	ImGui::Text(xorstr_("Player Alpha: %i"), g_player_list.alpha);
						ImGui::Text(xorstr_("Player Speed: %i MPH"), g_player_list.speed);
						//ImGui::Text(xorstr_("Player Location: %"), HUD::GET_STREET_NAME_FROM_HASH_KEY(g_player_list.zone));
					

						ImGui::Text(xorstr_("IP: %d.%d.%d.%d:%d"), g_player_list.online_ip.m_field1, g_player_list.online_ip.m_field2, g_player_list.online_ip.m_field3, g_player_list.online_ip.m_field4, g_player_list.online_port);
						
						ImGui::EndGroup();

						ImGui::NextColumn();

						ImGui::BeginGroup();
						ImGui::Text(xorstr_("Is Alive: ")); ImGui::SameLine();
						if(g_player_list.health < 1)
							ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(0, 180, 0, 255)), xorstr_("No"));
						else
							ImGui::Text(xorstr_("Yes"));
						ImGui::NewLine();
						
						ImGui::Text(xorstr_("Health: %f"), g_player_list.health);
						ImGui::Text(xorstr_("Armour: %f"), g_player_list.armour);
						
						

						
						

						ImGui::EndGroup();

						ImGui::Columns(1);
						ImGui::Separator();
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			else
				ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(ImColor(0, 0, 255)), xorstr_("Connect to GTA Online"));
		}
	} 
}