#pragma once
#include "script.hpp"

namespace zyko
{
	class MainScript
	{
	public:
		void gui_init();
		void gui_on_tick();

		void script_init();
		void script_on_tick();
		static void script_func();
	private:
		bool m_Initialized{};
		bool m_opened{};
	};

	inline MainScript g_MainScript;
}