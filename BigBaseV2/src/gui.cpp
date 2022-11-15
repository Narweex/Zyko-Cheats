#include "common.hpp"
#include "gui.hpp"
#include "logger.hpp"
#include "memory/module.hpp"
#include "memory/pattern.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script.hpp"
#include <imgui.h>
#include "auth/auth.hpp"
#include "gui/components/components.hpp"
#include <gui/list/UIManager.hpp>
#include "gui/imgui_tabs.h"
#include "style.hpp"

namespace zyko
{
	void gui::dx_init()
	{
		auto& style = ImGui::GetStyle();
		style.WindowPadding = { 10.f, 10.f };
		style.PopupRounding = 0.f;
		style.FramePadding = { 8.f, 0.5f };
		style.ItemSpacing = { 10.f, 8.f };
		style.ItemInnerSpacing = { 6.f, 4.f };
		style.TouchExtraPadding = { 0.f, 0.f };
		style.IndentSpacing = 21.f;
		style.ScrollbarSize = 15.f;
		style.GrabMinSize = 8.f;
		style.WindowBorderSize = 4.f;
		style.ChildBorderSize = 4.f;
		style.PopupBorderSize = 4.f;
		style.FrameBorderSize = 0.f;
		style.TabBorderSize = 0.f;
		style.WindowRounding = 4.f;
		style.ChildRounding = 4.f;
		style.FrameRounding = 4.f;
		style.ScrollbarRounding = 4.f;
		style.GrabRounding = 4.f;
		style.TabRounding = 4.f;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.DisplaySafeAreaPadding = { 3.f, 3.f };

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImColor(255, 255, 255);
		colors[ImGuiCol_TextDisabled] = ImColor(255, 255, 255);
		colors[ImGuiCol_WindowBg] = ImColor(19, 14, 24, customization.background_colour);
		colors[ImGuiCol_ChildBg] = ImColor(70, 70, 70, 100);
		colors[ImGuiCol_PopupBg] = ImColor(19, 14, 24, 240);
		colors[ImGuiCol_Border] = ImColor(120, 0, 255);
		colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0);
		colors[ImGuiCol_FrameBg] = ImColor(120, 0, 255, 108);
		colors[ImGuiCol_FrameBgHovered] = ImColor(120, 0, 255, 108);
		colors[ImGuiCol_FrameBgActive] = ImColor(120, 0, 255, 108);
		colors[ImGuiCol_ScrollbarBg] = ImColor(0, 0, 0, 0);
		colors[ImGuiCol_ScrollbarGrab] = ImColor(158, 0, 255);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(192, 0, 238);
		colors[ImGuiCol_ScrollbarGrabActive] = ImColor(192, 0, 238);
		colors[ImGuiCol_CheckMark] = ImColor(158, 0, 255);
		colors[ImGuiCol_SliderGrab] = ImColor(158, 0, 255);
		colors[ImGuiCol_SliderGrabActive] = ImColor(192, 0, 238);
		colors[ImGuiCol_Button] = ImColor(152, 0, 255, 183);
		colors[ImGuiCol_ButtonHovered] = ImColor(142, 0, 255, 230);
		colors[ImGuiCol_ButtonActive] = ImColor(142, 0, 255, 255);
		colors[ImGuiCol_Header] = ImColor(152, 0, 255, 154);
		colors[ImGuiCol_HeaderHovered] = ImColor(169, 0, 255, 160);
		colors[ImGuiCol_HeaderActive] = ImColor(169, 0, 255, 160);
		colors[ImGuiCol_Separator] = ImColor(152, 0, 255);
		colors[ImGuiCol_Tab] = ImColor(142, 0, 255);
		colors[ImGuiCol_TabActive] = ImColor(115, 0, 193, 255);
		colors[ImGuiCol_NavHighlight] = ImColor(120, 0, 255, 255);
	}

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;

	void gui::dx_on_tick()
	{
		TRY_CLAUSE
		{
			// Main Window
			ImGui::SetNextWindowSize(ImVec2(800.f, 630.f), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(400.f, 200.f), ImGuiCond_FirstUseEver);

			if (ImGui::Begin(xorstr_("##window"), NULL, flags))
			{
				// Window
				g_gui.g_window = ImGui::GetWindowPos();

				// H/W
				g_gui.height = ImGui::GetWindowHeight();
				g_gui.width = ImGui::GetWindowWidth();

				// Tabs Render
				ImGui::SameLine(160.f);
				ImGui::BeginGroup();
				{
					switch (g_gui.g_tab)
					{
					case 0:
						ImGuiTabs::render_base_tab();
						break;
					case 1:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_weapons_tab();
						}
						break;
					case 2:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_vehicle_tab();
						}
						break;
					case 3:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_teleport_tab();
						}
						break;
					case 4:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_lobby_tab();
						}
						break;
					case 5:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_players_tab();
						}
						break;
					case 6:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							if (ImGui::Button("unload")) {
								g_running = false;
							}
						}
						break;
					case 7:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_protections_tab();
						}
						break;
					case 8:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_recovery_tab();
						}
						break;
					case 9:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_misc_tab();
						}
						break;
					case 10:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_settings_tab();
						}
						break;
					case 11:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_spawn_tab();
						}
						break;
					case 12:
						if (!auth::login)
						{
							ImGui::Text(xorstr_("NoNoNo, Login First"));
						}
						else
						{
							ImGuiTabs::render_world_tab();
						}
							break;

						
					}
				}
				ImGui::EndGroup();
			}
			ImGui::End();

			ImGui::SetNextWindowPos(ImVec2(g_gui.g_window.x, g_gui.g_window.y), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(150.f, g_gui.height), ImGuiCond_Always);

			// ImGuiWindowFlags_NoBackground
			if (ImGui::Begin(xorstr_("##tabs"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			{
				// Tabs
				struct tabs
				{
					const char* icon{};
					int num{};
				};

				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.f));

				std::vector<tabs> tabs_v
				{
					{ ICON_FA_USER" Self", 0},
					{ ICON_FK_BOMB" Weapon", 1 },
					{ ICON_FK_CAR" Vehicle", 2 },
					{ ICON_FK_MAP_MARKER" Teleport", 3 },
					
				};

				std::vector<tabs> tabs_e
				{
					
					{ ICON_FK_SITEMAP" Lobby", 4 },
					{ ICON_FK_USERS" Players", 5 },
					{ ICON_FA_ETHERNET" Online", 6 },
					{ ICON_FA_SHIELD_ALT" Protections", 7 },
					{ ICON_FA_MONEY_BILL" Recovery", 8 },
					
				};

				std::vector<tabs> tabs_b
				{
					{ ICON_FA_ELLIPSIS_H" Misc", 9 },
					{ ICON_FK_COG" Settings", 10 },
					
					{ ICON_FK_PLUS_CIRCLE" Spawn", 11 },
					{ ICON_FA_GLOBE" World", 12 },
				};

				


				components::small_text(xorstr_("Self/Basic"));
				// Tabs
				for (const auto& rs : tabs_v)
				{
					ImGui::Spacing();
					if (ImGui::Selectable(rs.icon, g_gui.g_tab == rs.num, NULL))
					{
						g_gui.g_tab = rs.num;
					}	
				}

				components::small_text(xorstr_("Online"));

				for (const auto& rs : tabs_e)
				{
					ImGui::Spacing();
					if (ImGui::Selectable(rs.icon, g_gui.g_tab == rs.num, NULL))
					{
						g_gui.g_tab = rs.num;
					}	
				}

				components::small_text(xorstr_("Other"));

				for (const auto& rs : tabs_b)
				{
					ImGui::Spacing();
					if (ImGui::Selectable(rs.icon, g_gui.g_tab == rs.num, NULL))
					{
						g_gui.g_tab = rs.num;
					}
				}
				components::small_text(xorstr_(ZykoVersion));

				/*components::small_text(xorstr_("poggo text"));

				for (const auto& rs : tabs_a)
				{
					ImGui::Spacing();
					if (ImGui::Selectable(rs.icon, g_gui.g_tab == rs.num, NULL))
					{
						g_gui.g_tab = rs.num;
					}
				}*/

				ImGui::PopStyleVar();

			}
			ImGui::End();
		}
		EXCEPT_CLAUSE
	}

	void gui::script_init()
	{
	}

	void gui::script_on_tick()
	{
		TRY_CLAUSE
		{
			if (g_gui.m_opened && !g_list)
			{
				for (uint8_t i = 0; i <= 6; i++)
					PAD::DISABLE_CONTROL_ACTION(2, i, true);

				PAD::DISABLE_CONTROL_ACTION(2, 106, true);
				PAD::DISABLE_CONTROL_ACTION(2, 329, true);
				PAD::DISABLE_CONTROL_ACTION(2, 330, true);

				PAD::DISABLE_CONTROL_ACTION(2, 14, true);
				PAD::DISABLE_CONTROL_ACTION(2, 15, true);
				PAD::DISABLE_CONTROL_ACTION(2, 16, true);
				PAD::DISABLE_CONTROL_ACTION(2, 17, true);
				PAD::DISABLE_CONTROL_ACTION(2, 24, true);
				PAD::DISABLE_CONTROL_ACTION(2, 69, true);
				PAD::DISABLE_CONTROL_ACTION(2, 70, true);
				PAD::DISABLE_CONTROL_ACTION(2, 84, true);
				PAD::DISABLE_CONTROL_ACTION(2, 85, true);
				PAD::DISABLE_CONTROL_ACTION(2, 99, true);
				PAD::DISABLE_CONTROL_ACTION(2, 92, true);
				PAD::DISABLE_CONTROL_ACTION(2, 100, true);
				PAD::DISABLE_CONTROL_ACTION(2, 114, true);
				PAD::DISABLE_CONTROL_ACTION(2, 115, true);
				PAD::DISABLE_CONTROL_ACTION(2, 121, true);
				PAD::DISABLE_CONTROL_ACTION(2, 142, true);
				PAD::DISABLE_CONTROL_ACTION(2, 241, true);
				PAD::DISABLE_CONTROL_ACTION(2, 261, true);
				PAD::DISABLE_CONTROL_ACTION(2, 257, true);
				PAD::DISABLE_CONTROL_ACTION(2, 262, true);
				PAD::DISABLE_CONTROL_ACTION(2, 331, true);
			}
		}
		EXCEPT_CLAUSE
	}

	void gui::script_func()
	{
		g_gui.script_init();
		while (true)
		{
			g_gui.script_on_tick();
			script::get_current()->yield();
		}
	}
}
