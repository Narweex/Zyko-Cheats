#pragma once
#include "common.hpp"
#include <imgui.h>
#include "../../BigBaseV2/src/core/icons.h"

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
		
	public:
		ImFont* m_font;
		ImFont* m_monospace_font;
		ImFont* m_font_awesome;
		ImFont* m_notosans_regular;
		ImFont* m_staanliches;
		ImFont* m_title_font;
		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	private:
		comptr<IDXGISwapChain> m_dxgi_swapchain;
		comptr<ID3D11Device> m_d3d_device;
		comptr<ID3D11DeviceContext> m_d3d_device_context;
	};

	inline renderer *g_renderer{};
}
