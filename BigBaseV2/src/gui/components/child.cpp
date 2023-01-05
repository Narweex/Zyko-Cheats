#include "gui/components/components.hpp"

namespace zyko
{
	void components::child(const std::string title, const ImVec2& size, ImGuiWindowFlags extra_flags)
	{
		std::string tag = xorstr_("##");
		std::string id = tag + title;

		ImGui::BeginChild(id.c_str(), size, false, extra_flags);

		if (!title.empty())
		{
			ImGui::SetCursorPos(ImVec2(8.f, 9.f));
			small_text(title.data());
		}

		ImGui::SetCursorPosX(8);
		ImGui::BeginGroup();
	}

	void components::end_child()
	{
		ImGui::EndGroup();
		ImGui::EndChild();
	}
}