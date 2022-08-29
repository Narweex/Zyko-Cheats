#include "common.hpp"
#include "vehicle_tab.h"
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
	void vehicle_tab::render_vehicle_tab()
	{
		ImGui::BeginChild("##options1", ImVec2(300, 280), true);

		if (ImGui::Button(xorstr_("Max Tuning"), ImVec2(200, 25))) {int Vehicle; Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(features::g_selected_player), 0);features::maxvehicle(Vehicle); }
		if (ImGui::Button(xorstr_("Repair Vehicle"), ImVec2(200, 25))) { features::repairVehicle(); }
		if (ImGui::Button(xorstr_("Clean Vehicle"), ImVec2(200, 25))) { features::cleanVehicle(); }
		ImGui::Checkbox(xorstr_("Vehicle Godmode"), &features::vehgodmode);
		ImGui::Checkbox(xorstr_("Repair Loop"), &features::fixloop);
		ImGui::Checkbox(xorstr_("Clean Loop"), &features::cleanloop);
		ImGui::Checkbox(xorstr_("Vehicle Fly"), &features::noclip);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##options2", ImVec2(300, 280), true);

		if (ImGui::Button(xorstr_("Random Colour"), ImVec2(200, 25))) { }

		if (ImGui::Button(xorstr_("Repair Vehicle"), ImVec2(200, 25))) { features::repairVehicle(); }
		if (ImGui::Button(xorstr_("Clean Vehicle"), ImVec2(200, 25))) { features::cleanVehicle(); }
		ImGui::Checkbox(xorstr_("Vehicle Godmode"), &features::vehgodmode);
		ImGui::Checkbox(xorstr_("Repair Loop"), &features::fixloop);
		ImGui::Checkbox(xorstr_("Clean Loop"), &features::cleanloop);
		ImGui::Checkbox(xorstr_("Vehicle Fly"), &features::noclip);
		ImGui::EndChild();
	}

}