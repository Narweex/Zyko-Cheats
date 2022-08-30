#include "common.hpp"
#include "list_playerinfo.h"
#include "imgui.h"
#include "script.hpp"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "gta_util.hpp"
#include "gui/list/UIManager.hpp"
#include "features.hpp"
#include "script_global.hpp"

#include <iostream>
#include<cstring>
#

namespace big
{
	void list_playerinfo::render_playerinfo()
	{
		ImGui::SetNextWindowSize(ImVec2(0.5, 0.5));
		if (ImGui::BeginTabItem(PLAYER::GET_PLAYER_NAME(features::g_selected_player)))
		{
			ImGui::Text("Helo");
		}
	}
}