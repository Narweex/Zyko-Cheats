#include "common.hpp"
#include "self_tab.h"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "gui/list/UIManager.hpp"
#include "features.hpp"
#include "script_global.hpp"

namespace big
{
	void self_tab::render_self_tab()
	{

		if (ImGui::BeginTabItem("Self"))
		{
			

			ImGui::Spacing();

			ImGui::Checkbox("Godmode", &features::godmode);

			

		

		



		

			ImGui::Spacing();



			if (ImGui::Button("List Mode"))
			{
				g_list = true;
				g_UiManager->m_Opened = true;
			}
			ImGui::Separator();

			if (ImGui::Button("Unload"))
			{
				g_running = false;
			}

			ImGui::EndTabItem();
		}
	}
}