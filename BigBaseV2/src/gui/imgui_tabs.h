#pragma once

namespace zyko
{
	class ImGuiTabs
	{
	public:
		static void render_base_tab();
		static void render_weapons_tab();
		static void render_vehicle_tab();
		static void render_teleport_tab();
		static void render_lobby_tab();
		static void render_players_tab();
		static void render_protections_tab();
		static void render_recovery_tab();
		static void render_self_tab();
		static void render_misc_tab();
		static void render_spawn_tab();
		static void render_settings_tab();
		static void render_world_tab();
	};
}

