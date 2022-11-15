#include "gui/components/components.hpp"
#include "renderer.hpp"
#include "imgui.h"

namespace zyko
{
	void components::small_text(const std::string_view text)
	{
		ImVec4* colors = ImGui::GetStyle().Colors;

		if (!text.empty())
		{
			ImGui::PushFont(g_renderer->m_title_font);
			ImGui::TextColored(colors[ImGuiCol_Tab], text.data());
			ImGui::PopFont();
		}	
	}
}