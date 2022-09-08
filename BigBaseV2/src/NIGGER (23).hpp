#pragma once
#include "common.hpp"
#include "imgui.h"

namespace big
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();

		void script_init();
		void script_on_tick();
		static void script_func();
	public:
		bool m_opened{}; 
		int g_tab = 0;
		ImVec2 g_window;
		float height = 0.f, width = 0.f;
	};

	inline gui g_gui;
}
