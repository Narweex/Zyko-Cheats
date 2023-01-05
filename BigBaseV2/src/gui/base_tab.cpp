#include "imgui_tabs.h"
#include "features.hpp"
#include "fiber_pool.hpp"


namespace zyko
{
	void ImGuiTabs::render_base_tab()
	{
		if (!auth::login && !auth::login1)
		{		
			exit(0);
		}
		else
		{	
			ImGui::BeginChild(xorstr_("##options1"), ImVec2(300, 280), true);

			ImGui::Text(xorstr_("Self"));
			ImGui::Separator();
			ImGui::Checkbox(xorstr_("Godmode"), &features::godmode);
			ImGui::Checkbox(xorstr_("Semi Godmode"), &features::semigod);
			ImGui::Checkbox(xorstr_("No Ragdoll"), &features::norag);
			ImGui::Checkbox(xorstr_("Never Wanted"), &features::neverwanted);
			ImGui::SliderInt(xorstr_("Level"), &features::wantedLevel, 0, 5);
			if (ImGui::Button(xorstr_("Set Wanted Level"))) { features::setwanted(); }
			ImGui::Checkbox(xorstr_("Enter Vehicles Instantly"), &features::instantenter);
			ImGui::Checkbox(xorstr_("Forcefield"), &features::forcefield);
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild(xorstr_("##optionsmovement"), ImVec2(300, 280), true);
			ImGui::Text(xorstr_(" Movement"));
			ImGui::Separator();
			ImGui::Checkbox(xorstr_("Noclip"), &features::noclip);
			ImGui::Checkbox(xorstr_("Movement Override"), &features::superrunbool);
			ImGui::Checkbox(xorstr_("Super Run"), &features::ultrarunbool);
			ImGui::Checkbox(xorstr_("Super Jump"), &features::superjumpbool);
			ImGui::Checkbox(xorstr_("Ultra Jump"), &features::ultrajumpbool);
			ImGui::Checkbox(xorstr_("Flash Mode"), &features::flashrun);
			ImGui::Checkbox(xorstr_("Superman"), &features::superman);
			ImGui::Checkbox(xorstr_("Infinite Stamina"), &features::unlimitedstamina);

			ImGui::EndChild();


			ImGui::NewLine();
			ImGui::BeginChild(xorstr_("##optionsglobals"), ImVec2(300, 280), true);
			ImGui::Text(xorstr_(" Others"));
			ImGui::Separator();
			ImGui::Checkbox(xorstr_("Off Radar"), &features::offradar);
			ImGui::Checkbox(xorstr_("Bullshark testosterone"), &features::bullshark);
			ImGui::Checkbox(xorstr_("No Idle Kick"), &features::noidlekick);
			ImGui::Checkbox(xorstr_("Rotating player"), &features::rotating);
			ImGui::SliderFloat(xorstr_("Wetness"), &features::set_wetness, -1.0f, 1.0f);
			if(ImGui::Button(xorstr_("Set wetness")))
			{
				g_fiber_pool->queue_job([=] {
					features::Set_wetness(features::set_wetness);
					});
			}

			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::SameLine();
			ImGui::BeginChild(xorstr_("##optionsplayer"), ImVec2(300, 280), true);
			ImGui::Text(xorstr_(" Player"));
			ImGui::Separator();
			ImGui::SliderInt(xorstr_("Opacity"), &features::playeralpha, 0, 255);
			ImGui::Checkbox(xorstr_("Invisibility"), &features::invisibility);
			ImGui::Checkbox(xorstr_("Ignore Player"), &features::ignoreplayer);
			if (ImGui::Button(xorstr_("Clear Wanted Level"))) {
				g_fiber_pool->queue_job([=] {features::clearwanted; });
			}
			if (ImGui::Button(xorstr_("Suicide"))) {
				g_fiber_pool->queue_job([=] {features::suicide(); });
			}
			if (ImGui::Button(xorstr_("Reset Ped"))) {
				g_fiber_pool->queue_job([=] {features::resetped(); });
			}
			if (ImGui::Button(xorstr_("Max Health"))) {
				g_fiber_pool->queue_job([=] {features::maxhealth(); });
			};
			if (ImGui::Button(xorstr_("Max Armor"))) {
				g_fiber_pool->queue_job([=] {features::maxarmor(); });
			};
				ImGui::EndChild();
		}
	}
}