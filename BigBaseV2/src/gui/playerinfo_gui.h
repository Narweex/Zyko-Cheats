#pragma once
#include "common.hpp"
#include "imgui.h"

namespace big
{
	class playerinfo_gui
	{
	public:
		void dx_init();
		void dx_on_tick();

		void script_init();
		void script_on_tick();
		static void script_func();
	public:
		bool m_opened{};
		ImVec2 g_window;
		float height = 0.f, width = 0.f;
	};

	inline playerinfo_gui info_gui;
}
