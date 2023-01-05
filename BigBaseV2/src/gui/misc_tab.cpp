#include "imgui_tabs.h"
#include "features.hpp"
#include <notifications/notifications.h>
#include "fiber_pool.hpp"
#include "looped/gtaData.hpp"

namespace zyko
{
	static int selected_modifier = 0;
	static int selected_model = 0;
	void ImGuiTabs::render_misc_tab()
	{
		
		ImGui::PushItemWidth(200);
		if (ImGui::BeginCombo(xorstr_("Timecycles"), TimecycleModifiers[selected_modifier]))
		{
			if (ImGui::Selectable(xorstr_("Clear")))
			{
				g_fiber_pool->queue_job([=] {
					GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
					});
			}
			for (int i = 0; std::size(TimecycleModifiers); i++)
			{
				if (ImGui::Selectable(TimecycleModifiers[i]))
				{
					selected_modifier = i;
					g_fiber_pool->queue_job([=] {
						features::SetTimecycleModifier(TimecycleModifiers[selected_modifier]);
						
						});

				}
			}
		ImGui::EndCombo();
		}
		
		



		ImGui::SameLine();

		ImGui::PushItemWidth(200);
		if (ImGui::BeginCombo(xorstr_("Models"), PedModels[selected_model]))
		{
			
			for (int i = 0; i < std::size(PedModels); i++)
			{
				if (ImGui::Selectable(PedModels[i]))
				{
					selected_model = i;
					g_fiber_pool->queue_job([=] {
						features::changemodel(PedModels[selected_model]);
						});

				}
			}
			ImGui::EndCombo();
		}
		ImGui::NewLine();
		if (ImGui::BeginCombo(xorstr_("Animations"), xorstr_("Animations")))
		{
			for (auto& anims : animations)
			{
				if (ImGui::Selectable(anims.label))
				{
					g_fiber_pool->queue_job([=] {features::PlayAnimation(anims.name, anims.id);	});
				}

			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo(xorstr_("Scenarios"), xorstr_("Scenarios")))
		{
			for (auto& scenes : scenarioslol)
			{
				if (ImGui::Selectable(scenes.id))
				{
					g_fiber_pool->queue_job([=] {features::PlayScenario(scenes.id);	});
				}

			}
			ImGui::EndCombo();
		}
		

		ImGui::NewLine();

		if (ImGui::Button(xorstr_("Stop Animation/Scenario")))
		{
			g_fiber_pool->queue_job([=] {
				features::StopAnimation();
				});
		}

		if (ImGui::Button(xorstr_("Unfreeze player")))
		{
			g_fiber_pool->queue_job([=] {
				features::Unfreeze();
				});
		}


	

		
		
		
		
	}
}