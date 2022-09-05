#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "libcurl_a.lib")

#include <SDKDDKVer.h>
#include <Windows.h>
#include <D3D11.h>
#include <wrl/client.h>

#include <cinttypes>
#include <cstddef>
#include <cstdint>

#include <chrono>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <atomic>
#include <mutex>
#include <thread>

#include <memory>
#include <new>

#include <sstream>
#include <string>
#include <string_view>

#include <algorithm>
#include <functional>
#include <utility>

#include <stack>
#include <vector>

#include <typeinfo>
#include <type_traits>

#include <exception>
#include <stdexcept>

#include <any>
#include <optional>
#include <variant>

#include <fmt/format.h>
#include <nlohmann/json.hpp>
#define JM_XORSTR_DISABLE_AVX_INTRINSICS
#include "logger.hpp"
#include "core/xor_string.hpp"
#include "core/config.h"
#include "auth/auth.hpp"
#include "auth/auth.hpp"
#include "gui/base_tab.h"
#include "gui/players_tab.h"
#include "gui/self_tab.h"
#include "gui/components/components.hpp"
#include <gui/list/UIManager.hpp>
#include "gui/spawn_tab.h"
#include "gui/weapons_tab.h"
#include "gui/lobby_tab.h"
#include "gui/vehicle_tab.h"
#include "gui/list/Lists.hpp"
//#include "gui/spoof_tab.h"
#include "natives.hpp"
#include "gta_util.hpp"
#include "script_global.hpp"
#include "script.hpp"
#include "helpers/other.h"
#include <gta/ped_factory.hpp>
#include <shellapi.h>
#include "auth/lazy_importer.hpp"





#define _PTR_MAX_VALUE ((PVOID)0x000F000000000000)

constexpr bool IsValidPtr(PVOID p)
{
	return (p >= (PVOID)0x10000) && (p < _PTR_MAX_VALUE) && p != nullptr;
}

#define MIN_VTABLE ((uint64_t)0x140000000)
#define MAX_VTABLE ((uint64_t)0x14FFFFFFF)

constexpr bool IsValidPtrWithVTable(void* p)
{
	if (IsValidPtr(p))
	{
		void* vtable = *reinterpret_cast<void**>(p);
		if (IsValidPtr(vtable) && reinterpret_cast<uint64_t>(vtable) > MIN_VTABLE && reinterpret_cast<uint64_t>(vtable) < MAX_VTABLE)
			return true;
	}

	return false;
}

namespace big
{
	using namespace std::chrono_literals;
	
	template <typename T>
	using comptr = Microsoft::WRL::ComPtr<T>;

	inline HMODULE g_hmodule{};
	inline HANDLE g_main_thread{};
	inline DWORD g_main_thread_id{};
	inline std::atomic_bool g_running{ true };
	inline CPed* g_local_player;
	inline bool g_first{ false };
	inline bool g_list{ false };
}
