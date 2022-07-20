#pragma once
#include "imgui.h"

namespace big
{
	class components
	{
	public:
		static bool button(const std::string_view);
		static bool button(const std::string_view, const ImVec2& size);
		static void small_text(const std::string_view text);
		static void button(const std::string_view, std::function<void()>);
		static void button(const std::string_view, const ImVec2& size, std::function<void()>);
		static void child(const std::string title, const ImVec2& size, ImGuiWindowFlags extra_flags = 0);
		static void end_child();
	};
}