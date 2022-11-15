#pragma once
#include "imgui_notify.h"
namespace zyko
{
	enum NotifyLevel
	{
		None = 1,
		Success,
		Protections,
		Error
	};
	void Notify(const char* title, const char* text, uint32_t duration, int level);
}
