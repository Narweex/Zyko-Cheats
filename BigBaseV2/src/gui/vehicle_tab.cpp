#include "common.hpp"
#include "imgui_tabs.h"
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
	void ImGuiTabs::render_vehicle_tab()
	{
		ImGui::BeginChild("##options1", ImVec2(300, 280), true);

		if (ImGui::Button(xorstr_("Max Tuning"), ImVec2(200, 25))) {int Vehicle; Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(features::g_selected_player), 0); }
		if (ImGui::Button(xorstr_("Repair Vehicle"), ImVec2(200, 25))) {g_fiber_pool->queue_job([=] {features::repairVehicle();});}
		if (ImGui::Button(xorstr_("Clean Vehicle"), ImVec2(200, 25))) { features::cleanVehicle(); }
		ImGui::Checkbox(xorstr_("Vehicle Godmode"), &features::vehgodmode);
		ImGui::Checkbox(xorstr_("Repair Loop"), &features::fixloop);
		ImGui::Checkbox(xorstr_("Clean Loop"), &features::cleanloop);
		ImGui::Checkbox(xorstr_("Vehicle Fly"), &features::noclip);
		ImGui::Checkbox(xorstr_("Rainbow Colour"), &features::rainbowcar);
		ImGui::Checkbox(xorstr_("Invisible Vehicle"), &features::invis_car);
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##options2", ImVec2(300, 280), true);

	
		
		ImGui::EndChild();
	}

}