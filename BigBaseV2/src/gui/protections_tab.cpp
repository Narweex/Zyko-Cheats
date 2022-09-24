#include "protections_tab.h"
#include "features.hpp"


namespace big
{
	void protections_tab::render_protections_tab()
	{
		ImGui::Checkbox("Notify for Blocks", &features::g_received_event);
		ImGui::Checkbox("Block Explosions", &features::g_explosion_event);
		ImGui::Checkbox("Block Reports", &features::no_report_event);
		ImGui::Checkbox("Block Particle Effects", &features::g_ptfx_event);
		ImGui::Checkbox("Log Net Events", &features::g_log_net_event_data);
		ImGui::Checkbox("No Freeze", &features::no_freeze_event);
		ImGui::Checkbox("No Pickup", &features::no_pickup_event);
		ImGui::Checkbox("No Clock Events", &features::no_clock_event);
		ImGui::Checkbox("No Sound Events", &features::no_sound_effect);
		ImGui::Checkbox("No Votekick", &features::no_votekick_event);
		ImGui::Checkbox("No Weapons Event", &features::no_weapons_event);
		ImGui::Checkbox("No Wanted Event", &features::no_wanted_event);
		ImGui::Checkbox("No Weather ", &features::no_weather_event);
		ImGui::Checkbox("Block scripted game events", &features::g_scripted_game_event);
		ImGui::Checkbox("Crash Protection", &features::g_crash_protex);
		ImGui::Checkbox("Kick Protection", &features::g_kick_protex);

	}
}

