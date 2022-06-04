#pragma once

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

#include "logger.hpp"
#include "core/xor_string.hpp"
#include "core/config.h"
#include "CPed.hpp"

#define _DLL_NAME "Zyko.dll" //to change in final release
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
