#pragma once
#include "common.hpp"
#include <imgui.h>
#include <set>
#include "core/icons.h"
typedef void(*PresentCallback)(void*);

namespace big
{
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		void on_present();

		void pre_reset();
		void post_reset();
		
		bool add_callback(PresentCallback callback) { return m_present_callbacks.insert(callback).second; }
		bool remove_callback(PresentCallback callback) { return m_present_callbacks.erase(callback) != 0; }
	public:
		ImFont* m_font;
		ImFont* m_monospace_font;
		ImFont* m_font_awesome;
		ImFont* m_notosans_regular;
		ImFont* m_staanliches;
		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	private:
		comptr<IDXGISwapChain> m_dxgi_swapchain;
		comptr<ID3D11Device> m_d3d_device;
		comptr<ID3D11DeviceContext> m_d3d_device_context;
		std::set<PresentCallback> m_present_callbacks;
	};

	inline renderer *g_renderer{};
}
