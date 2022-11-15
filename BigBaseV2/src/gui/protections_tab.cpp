#include "imgui_tabs.h"
#include "features.hpp"


namespace zyko
{
	void ImGuiTabs::render_protections_tab()
	{
		ImGui::Checkbox(xorstr_("Notify for Blocks"), &features::g_received_event);
		ImGui::Checkbox(xorstr_("Block Explosions"), &features::g_explosion_event);
		ImGui::Checkbox(xorstr_("Block Reports"), &features::no_report_event);
		ImGui::Checkbox(xorstr_("Block Particle Effects"), &features::g_ptfx_event);
		ImGui::Checkbox(xorstr_("Log Net Events"), &features::g_log_net_event_data);
		ImGui::Checkbox(xorstr_("No Freeze"), &features::no_freeze_event);
		ImGui::Checkbox(xorstr_("No Pickup"), &features::no_pickup_event);
		ImGui::Checkbox(xorstr_("No Clock Events"), &features::no_clock_event);
		ImGui::Checkbox(xorstr_("No Sound Events"), &features::no_sound_effect);
		ImGui::Checkbox(xorstr_("No Votekick"), &features::no_votekick_event);
		ImGui::Checkbox(xorstr_("No Weapons Event"), &features::no_weapons_event);
		ImGui::Checkbox(xorstr_("No Wanted Event"), &features::no_wanted_event);
		ImGui::Checkbox(xorstr_("No Weather "), &features::no_weather_event);
		ImGui::Checkbox(xorstr_("Block scripted game events"), &features::g_scripted_game_event);
		ImGui::Checkbox(xorstr_("Crash Protection"), &features::g_crash_protex);
		ImGui::Checkbox(xorstr_("Kick Protection"), &features::g_kick_protex);

	}
}

