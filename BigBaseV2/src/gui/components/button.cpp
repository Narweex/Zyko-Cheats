#include "gui/components/components.hpp"
#include "fiber_pool.hpp"

namespace big
{
	bool components::button(const std::string_view text) {
		return ImGui::Button(text.data());
	}
	bool components::button(const std::string_view text, const ImVec2& size) {
		return ImGui::Button(text.data(), size);
	}

	void components::button(const std::string_view text, std::function<void()> cb) {
		if (components::button(text)) {
			g_fiber_pool->queue_job(cb);
		}
	}
	void components::button(const std::string_view text, const ImVec2& size, std::function<void()> cb) {
		if (components::button(text, size)) {
			g_fiber_pool->queue_job(cb);
		}
	}
}