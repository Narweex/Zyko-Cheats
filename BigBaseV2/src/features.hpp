#pragma once
#include "common.hpp"

namespace big::features
{
	void run_tick();
	void script_func();
	void on_present();

	inline ULONGLONG ticks[] = { 0, 0, 0, 0 };
	inline ULONGLONG tick_conf[] = { 50, 250, 2000, 25003 };
	inline int g_current_tick_job{};

	/* Self */
}
