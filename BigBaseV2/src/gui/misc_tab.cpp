#include "imgui_tabs.h"
#include "features.hpp"
#include <notifications/notifications.h>
#include "fiber_pool.hpp"
#include "looped/gtaData.hpp"

namespace zyko
{
	void ImGuiTabs::render_misc_tab()
	{
		int selected_modifier = 0;
		ImGui::PushItemWidth(200);
		if (ImGui::BeginCombo(xorstr_("Timecycle modifiers"), TimecycleModifiers[selected_modifier]))
		{
			if (ImGui::Selectable("clear"))
			{
				g_fiber_pool->queue_job([=] {
					GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
					});
			}
			for (int i = 0; i < 32; i++)
			{
				if (ImGui::Selectable(TimecycleModifiers[i]))
				{
					selected_modifier = i;
					LOG(INFO) << TimecycleModifiers[i];
					g_fiber_pool->queue_job([=] {
						GRAPHICS::SET_TIMECYCLE_MODIFIER((TimecycleModifiers[selected_modifier]));
						});

				}
			}
		}

		ImGui::EndCombo();


		ImGui::SameLine();

		ImGui::BeginChild(xorstr_("##modelchanger"), ImVec2(300, 280), true);
		if (ImGui::TreeNode(xorstr_("Model Changer")))
		{
			for (auto models : PedModels)
			{
				if (ImGui::Button(models))
				{
					g_fiber_pool->queue_job([=] {features::changemodel(models);	});
				}
			}
		}
		ImGui::EndChild();

		ImGui::NewLine();
		ImGui::Text(xorstr_("In Testing"));
		for (auto& anims : animations)
		{
			if (ImGui::Button(anims.name))
			{
				g_fiber_pool->queue_job([=] {features::PlayAnimation(anims.name, anims.id);	});
			}

		}

		if (ImGui::Button(xorstr_("stop anim")))
		{
			g_fiber_pool->queue_job([=] {
				features::StopAnimation();
				});
		}


		if (ImGui::Button(xorstr_("Spawn cat companion")))
		{
			g_fiber_pool->queue_job([=] {
				features::Spawn_companion();
				});
		}

		/*const char* sample[]
		{
			"Sample1",
			"Sample2",
			"Sample3"
		};

		static int selected = 0;
		if (ImGui::BeginCombo("Combo", sample[selected]))
		{
			for (int i = 0; i < 3; i++)
			{
				if (ImGui::Selectable(sample[i]))
				{
					selected = i;
					LOG(INFO) << sample[i];
				}
			}
		}
		ImGui::EndCombo();*/
		
		
		
	}
}