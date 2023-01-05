#include "imgui_tabs.h"
#include "imgui.h"
#include "fiber_pool.hpp"
#include "natives.hpp"
#include "features.hpp"
#include "gui/list/Lists.hpp"


namespace zyko
{
	void ImGuiTabs::render_spawn_tab()
	{
		static char vehicle[28];
		ImGui::Checkbox("Spawn In Vehicle", &features::in_vehicle);
		ImGui::Checkbox("Spawn Invincible", &features::vehicle_godmode);
		ImGui::Checkbox("Spawn Maxed", &features::full_stats);

		ImGui::InputText("Spawn by name", vehicle, sizeof(vehicle));
		if (ImGui::Button("Spawn"))
		{
			g_fiber_pool->queue_job([=] {
				features::spawn_veh(rage::joaat(vehicle));
				});
		}

		if (ImGui::BeginCombo(xorstr_("Super"), xorstr_("Super")))
		{
			for (auto& super : Lists::Super1)
			{
				if (ImGui::Selectable(super))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(super)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Sports"), xorstr_("Sports")))
		{
			for (auto& sports : Lists::Sports1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Sports Classics"), xorstr_("Sports Classics")))
		{
			for (auto& sports : Lists::SportsClassics1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Sedans"), xorstr_("Sedans")))
		{
			for (auto& sports : Lists::Sedans1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Service"), xorstr_("Service")))
		{
			for (auto& sports : Lists::Service1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Planes"), xorstr_("Planes")))
		{
			for (auto& sports : Lists::Planes1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Offroad"), xorstr_("Offroad")))
		{
			for (auto& sports : Lists::OffRoad1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Muscle"), xorstr_("Muscle")))
		{
			for (auto& sports : Lists::Muscle1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Motorcycles"), xorstr_("Motorcycles")))
		{
			for (auto& sports : Lists::Motorcycles1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Military"), xorstr_("Military")))
		{
			for (auto& sports : Lists::Military1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Industrial"), xorstr_("Industrial")))
		{
			for (auto& sports : Lists::Industrial1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Helicopters"), xorstr_("Helicopters")))
		{
			for (auto& sports : Lists::Helicopters1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Emergency"), xorstr_("Emergency")))
		{
			for (auto& sports : Lists::Emergency1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Cycles"), xorstr_("Cycles")))
		{
			for (auto& sports : Lists::Cycles1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Coupes"), xorstr_("Coupes")))
		{
			for (auto& sports : Lists::Coupes1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Compacts"), xorstr_("Compacts")))
		{
			for (auto& sports : Lists::Compacts1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Commercial"), xorstr_("Commercial")))
		{
			for (auto& sports : Lists::Commercial1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Boats"), xorstr_("Boats")))
		{
			for (auto& sports : Lists::Boats1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("SUVs"), xorstr_("SUVs")))
		{
			for (auto& sports : Lists::SUVs1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Utility"), xorstr_("Utility")))
		{
			for (auto& sports : Lists::Utility1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
		if (ImGui::BeginCombo(xorstr_("Vans"), xorstr_("Vans")))
		{
			for (auto& sports : Lists::Vans1)
			{
				if (ImGui::Selectable(sports.c_str()))
				{
					g_fiber_pool->queue_job([=] {features::spawn_veh(rage::joaat(sports)); });
				}
			}
		}
	}
}