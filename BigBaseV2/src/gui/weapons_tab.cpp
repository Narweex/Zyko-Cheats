#include "common.hpp"
#include "weapons_tab.h"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "gui/list/UIManager.hpp"
#include "features.hpp"
#include "script_global.hpp"
#include "auth/auth.hpp"
#include <iostream>
#include<cstring>
#include <helpers/other.h>


namespace big
{
	void weapons_tab::render_weapons_tab()
	{
		ImGui::BeginChild("##options1", ImVec2(300, 280), true);

		if (ImGui::Button(xorstr_("Give All Weapons"), ImVec2(200, 25))) { features::GiveAllWeapons(); }
		if (ImGui::Button(xorstr_("Remove All Weapons"), ImVec2(200, 25))) { features::RemoveAllWeapons(); }
		if (ImGui::Button(xorstr_("Give Max Ammo"), ImVec2(200, 25))) { features::MaxAmmo(); }
		ImGui::Checkbox(xorstr_("Infinite Ammo"), &features::infiniteammo);
		ImGui::Checkbox(xorstr_("Delete Gun"), &features::deletegun);
		ImGui::Checkbox(xorstr_("Aimbot"), &features::aimbot);
		ImGui::Checkbox(xorstr_("Explosive Gun"), &features::exploammo);
		ImGui::Checkbox(xorstr_("Airstrike Gun"), &features::airstrikegun);
		ImGui::Checkbox(xorstr_("Teleport Gun"), &features::teleportgun);
		ImGui::Checkbox(xorstr_("Drive It Gun"), &features::driveitgun);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##options2", ImVec2(300, 280), true);

		if (ImGui::Button(xorstr_("Give All Weapons"), ImVec2(200, 25))) { features::GiveAllWeapons(); }
		if (ImGui::Button(xorstr_("Remove All Weapons"), ImVec2(200, 25))) { features::RemoveAllWeapons(); }
		if (ImGui::Button(xorstr_("Give Max Ammo"), ImVec2(200, 25))) { features::MaxAmmo(); }
		ImGui::Checkbox(xorstr_("Infinite Ammo"), &features::infiniteammo);
		ImGui::Checkbox(xorstr_("Delete Gun"), &features::deletegun);
		ImGui::Checkbox(xorstr_("Aimbot"), &features::aimbot);
		ImGui::Checkbox(xorstr_("Explosive Gun"), &features::exploammo);
		ImGui::Checkbox(xorstr_("Airstrike Gun"), &features::airstrikegun);
		ImGui::Checkbox(xorstr_("Teleport Gun"), &features::teleportgun);
		ImGui::Checkbox(xorstr_("Drive It Gun"), &features::driveitgun);
		ImGui::EndChild();


		ImGui::BeginChild("##options3", ImVec2(300, 280), true);
		ImGui::SliderInt(xorstr_("Field Of View"), &features::fieldofview, 0, 360);
		ImGui::EndChild();
	}
}
