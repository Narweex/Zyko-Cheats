#include "common.hpp"
#include "spawn_tab.h"
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
#include "list/Lists.hpp"


namespace big
{
	void spawn_tab::render_spawn_tab()
	{



		ImGui::Spacing();



		if (!auth::login)
		{
			ImGui::InputText("Username", username, sizeof(username));
			ImGui::InputText("Password", password, sizeof(password));
			if (ImGui::Button("Log In"))
			{
				LOG(INFO) << username;
				std::string username1(username);
				std::string password1(password);

				auth::auth(username1, password1);
			}
		}
		else
		{






			ImGui::BeginChild("##options1", ImVec2(300, 280), true);
			ImGui::Text("Super");
			
			/*for (auto &veh : Lists::Super1)
			{

				if (ImGui::Button(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat(veh)))))
				{
					features::spawn_veh(rage::joaat(veh));
				}
			}*/
			//if (ImGui::TreeNode(xorstr_("Cayo Perico Helper")))
			//{
			//	ImGui::Button("Neger");
			//	ImGui::Button("Neger");
			//	ImGui::Button("Neger");
			//	ImGui::Button("Neger");
			//	ImGui::Button("Neger");
			//	ImGui::Button("Neger");
			//	/*for (const auto vehicle : Lists::Super1);
			//	{
			//		if(ImGui::Button(HUD::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(rage::joaat()))))
			//		{
			//			features::spawn_veh(rage::joaat(car));
			//		}
			//	}*/
			//}
			
			

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