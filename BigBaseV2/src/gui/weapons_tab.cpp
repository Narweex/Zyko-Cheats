#include "imgui_tabs.h"
#include "imgui.h"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "features.hpp"
#include "auth/auth.hpp"
#include <helpers/other.h>
#include "gta/Weapons.h"


namespace zyko
{
	void ImGuiTabs::render_weapons_tab()
	{
		ImGui::BeginChild(xorstr_("##options1"), ImVec2(300, 300), true);

		if (ImGui::Button(xorstr_("Give All Weapons"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {	features::GiveAllWeapons();  }); }
		if (ImGui::Button(xorstr_("Remove All Weapons"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {	features::RemoveAllWeapons();  }); }
		if (ImGui::Button(xorstr_("Give Max Ammo"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] { features::MaxAmmo(); }); }
		ImGui::Checkbox(xorstr_("Infinite Ammo"), &features::infiniteammo);
		ImGui::Checkbox(xorstr_("Delete Gun"), &features::deletegun);
		ImGui::Checkbox(xorstr_("Aimbot"), &features::aimbot);
		ImGui::Checkbox(xorstr_("Explosive Gun"), &features::exploammo);
		ImGui::Checkbox(xorstr_("Airstrike Gun"), &features::airstrikegun);
		ImGui::Checkbox(xorstr_("Teleport Gun"), &features::teleportgun);
		ImGui::Checkbox(xorstr_("Drive It Gun"), &features::driveitgun);
		ImGui::Checkbox(xorstr_("Rapid Fire"), &features::rapid_fire);

		ImGui::EndChild();
		ImGui::SameLine();
		
		ImGui::BeginChild(xorstr_("##options3"), ImVec2(300, 300), true);
		ImGui::Text(xorstr_("Give Weapon"));
		ImGui::Separator();
		for (auto& weapon : weaponlist)
		{
			if (ImGui::Selectable(weapon.label.c_str()))
			{
					g_fiber_pool->queue_job([=] { WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), rage::joaat(weapon.hash), 50, 1); });
				
			}
		}
		ImGui::EndChild();
	}
}
