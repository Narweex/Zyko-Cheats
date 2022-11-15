#include "imgui_tabs.h"
#include "features.hpp"
#include <notifications/notifications.h>
#include "fiber_pool.hpp"
#include "looped/gtaData.hpp"

namespace zyko
{
	void ImGuiTabs::render_misc_tab()
	{
		ImGui::BeginChild(xorstr_("##optionstimecycle"), ImVec2(300, 280), true);
		if (ImGui::TreeNode(xorstr_("Timecycle Modifiers")))
		{
			if (ImGui::Button(xorstr_("Clear")))
			{
				GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
			}
			for (auto modifiers : TimecycleModifiers)
			{
				if (ImGui::Button(modifiers))
				{
					g_fiber_pool->queue_job([=] {features::SetModifier(modifiers);	});
				}

			}
		}
		ImGui::EndChild();


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

		const char* sample[]
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

		/*static int selected = 0;
		ImGui::BeginCombo(xorstr_("Timecycle modifiers"), TimecycleModifiers[selected]);
		for (int i = 0; i < sizeof(TimecycleModifiers); i++)
		{
			if (ImGui::Selectable(TimecycleModifiers[i]), selected == i ? true : false)
			{
				selected = i;
				g_fiber_pool->queue_job([=] {
					features::SetModifier(TimecycleModifiers[selected]);
					});
			}
		}*/

		ImGui::EndCombo();

	}
}