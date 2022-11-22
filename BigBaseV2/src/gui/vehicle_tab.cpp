#include "common.hpp"
#include "imgui_tabs.h"
#include "imgui.h"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "features.hpp"


namespace zyko
{
	void ImGuiTabs::render_vehicle_tab()
	{
		ImGui::BeginChild(xorstr_("##options1"), ImVec2(300, 280), true);

		if (ImGui::Button(xorstr_("Max Tuning"), ImVec2(200, 25))) { int Vehicle; Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(features::g_selected_player), 0); }
		if (ImGui::Button(xorstr_("Repair Vehicle"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {features::repairVehicle(); }); }
		if (ImGui::Button(xorstr_("Clean Vehicle"), ImVec2(200, 25))) { features::cleanVehicle(); }
		ImGui::Checkbox(xorstr_("Vehicle Godmode"), &features::vehgodmode);
		ImGui::Checkbox(xorstr_("Repair Loop"), &features::fixloop);
		ImGui::Checkbox(xorstr_("Clean Loop"), &features::cleanloop);
		ImGui::Checkbox(xorstr_("Vehicle Fly"), &features::noclip);
		ImGui::Checkbox(xorstr_("Invisible Vehicle"), &features::invis_car);
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild(xorstr_("##options2"), ImVec2(300, 360), true);
		ImGui::Text(xorstr_("Vehicle Acrobatics"));
		ImGui::Separator();
		if (ImGui::Button(xorstr_("Frontflip"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {features::acrobatics(Frontflip); }); }
		if (ImGui::Button(xorstr_("Backflip"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {features::acrobatics(Backflip); }); }
		if (ImGui::Button(xorstr_("Left flip"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {features::acrobatics(LeftFlip); }); }
		if (ImGui::Button(xorstr_("Right flip"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {features::acrobatics(RightFlip); }); }
		if (ImGui::Button(xorstr_("Launch Up"), ImVec2(200, 25))) { g_fiber_pool->queue_job([=] {features::acrobatics(LaunchUp); }); }
		ImGui::SliderFloat(xorstr_("Force"), &features::z, -10.f, 10.f);
		ImGui::SliderFloat(xorstr_("Offset X"), &features::offX, -10.f, 10.f);
		ImGui::SliderFloat(xorstr_("Offset Y"), &features::offY, -10.f, 10.f);
		ImGui::SliderFloat(xorstr_("Offset Z"), &features::offZ, -10.f, 10.f);
		if (ImGui::Button(xorstr_("Do acrobatics")))
		{
			g_fiber_pool->queue_job([=]{features::perform();});
		}
		ImGui::SameLine();
		if(ImGui::Button("Reset"))
		{
			features::y = 0;
			features::offX = 0.f;
			features::offY = 0.f;
			features::offZ = 0.f;

		}
		
		


		ImGui::EndChild();

		ImGui::BeginChild(xorstr_("##colours"), ImVec2(300, 240), true);
		ImGui::Text(xorstr_("Vehicle Colors"));
		ImGui::Checkbox(xorstr_("Rainbow car"), &features::rainbowcar);
		ImGui::Checkbox(xorstr_("Rainbow car (flashing)"), &features::flashrainbow);
		ImGui::SliderInt(xorstr_("Red"), &features::VEHr, 0, 255);
		ImGui::SliderInt(xorstr_("Green"), &features::VEHg, 0, 255);
		ImGui::SliderInt(xorstr_("Blue"), &features::VEHb, 0, 255);
		if(ImGui::Button(xorstr_("Set this color (primary)")))
		{
			static const int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(VehID, features::VEHr, features::VEHg, features::VEHb);
		}
		if (ImGui::Button(xorstr_("Set this color (secondary)")))
		{
			static const int VehID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(VehID, features::VEHr, features::VEHg, features::VEHb);
		}
		ImGui::EndChild();
		ImGui::SameLine();
	
		ImGui::BeginChild(xorstr_("##misc"), ImVec2(300, 210), true);
		ImGui::InputText(xorstr_("Text"), features::number_plate, sizeof(features::number_plate));

		if (ImGui::Button(xorstr_("Set numberplate")))
		{
			features::SetNumberplate(features::number_plate);
		}
		ImGui::EndChild();
	}
}