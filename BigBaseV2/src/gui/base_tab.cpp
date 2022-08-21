#include "common.hpp"
#include "base_tab.h"
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


namespace big
{
	void base_tab::render_base_tab()
	{



		ImGui::Spacing();



		if (!auth::login)
		{
			
			std::string login_file = getenv("APPDATA");
			login_file += "\\Zyko\\autologin.json";
			const char* logfil = login_file.c_str();

			if (std::filesystem::exists(login_file))
			{
				std::string username2;
				std::string password2;
				nlohmann::json login;

				std::ifstream login_file(logfil, std::ifstream::binary);
				login_file >> login;
				
				username2 = login["Username"];
				password2 = login["Password"];

				auth::auth(username2, password2);
				





			}
			else
			{

			
				ImGui::InputText("Username", username, sizeof(username));
				ImGui::InputText("Password", password, sizeof(password));
			
				

			}
			
			if (ImGui::Button("Log In"))
			{
				LOG(INFO) << username;
				std::string username1(username);
				std::string password1(password);

				
				auth::connect();
				auth::authenticate();
				auth::auth(username1, password1);
				auth::verify();
				auth::log_in();
				auth::unlock();
			}
		}
		else
		{






			ImGui::BeginChild("##options1", ImVec2(300, 280), true);

			ImGui::Text(xorstr_(" Self"));
			ImGui::Checkbox(xorstr_("Godmode"), &features::godmode);
			ImGui::Checkbox(xorstr_("Semi Godmode"), &features::semigod);
			ImGui::Checkbox(xorstr_("No Ragdoll"), &features::norag);
			ImGui::Checkbox(xorstr_("Never Wanted"), &features::neverWanted);
			ImGui::SliderInt(xorstr_("Level"), &features::wantedLevel, 0, 5);
			if (ImGui::Button(xorstr_("Set Wanted Level"), ImVec2(200, 25))) { features::setwanted(); }
			ImGui::Checkbox(xorstr_("Enter Vehicles Instantly"), &features::instartenter);
			ImGui::Checkbox(xorstr_("Forcefield"), &features::forcefield);
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::BeginChild("##optionsmovement", ImVec2(300, 280), true);
			ImGui::Text(xorstr_(" Movement"));
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
			ImGui::BeginChild("##optionsglobals", ImVec2(300, 270), true);
			ImGui::Text(xorstr_(" Globals"));
			ImGui::Checkbox(xorstr_("Off Radar"), &features::offradar);
			ImGui::Checkbox(xorstr_("Bullshark testosterone"), &features::bullshark);
			//ImGui::Checkbox(xorstr_("Reveal Players"), &features::revealplayers);

			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::SameLine();
			ImGui::BeginChild("##optionsplayer", ImVec2(300, 270), true);
			ImGui::Text(xorstr_(" Player"));
			ImGui::SliderInt(xorstr_("Player Opacity"), &features::playeralpha, 0, 255);
			if (ImGui::Button("Clear Wanted Level")) { &features::clearwanted; }
			ImGui::Checkbox(xorstr_("Invisibility"), &features::invisibility);
			if (ImGui::Button("Suicide")) { features::suicide(); }
			if (ImGui::Button("Reset Ped")) { features::resetped(); }
			if (ImGui::Button("Max Health")) { features::maxhealth(); };
			if (ImGui::Button("Max Armor")) { features::maxarmor(); };

				ImGui::EndChild();








				/*ImGui::Spacing();

				if (ImGui::Button("List Mode"))
				{
					g_list = true;
					g_UiManager->m_Opened = true;
				}
				ImGui::Separator();

				if (ImGui::Button("Unload"))
				{
					g_running = false;
				}*/
			}
		}
	
}