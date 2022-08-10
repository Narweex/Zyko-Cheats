#pragma once
#include "common.hpp"
#include "gta/fwddec.hpp"
#include "gta/enums.hpp"
#include "function_types.hpp"

namespace big
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();
	public:
		HWND m_hwnd{};
		functions::file_register_t m_file_register{};

		using TriggerScriptEvent = int(int unk0, uint64_t* args, int argCount, int bitFlags);
		TriggerScriptEvent* m_TriggerScriptEvent;

		eGameState* m_game_state{};
		bool* m_is_session_started{};

		using FindScriptEvent = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
		FindScriptEvent* m_get_event_data;

		CPedFactory** m_ped_factory{};
		CNetworkPlayerMgr** m_network_player_mgr{};

		rage::scrNativeRegistrationTable* m_native_registration_table{};
		functions::get_native_handler_t m_get_native_handler{};
		functions::fix_vectors_t m_fix_vectors{};

		rage::atArray<GtaThread*>* m_script_threads{};
		rage::scrProgramTable* m_script_program_table{};
		functions::run_script_threads_t m_run_script_threads{};
		std::int64_t** m_script_globals{};

		CGameScriptHandlerMgr** m_script_handler_mgr{};

		IDXGISwapChain** m_swapchain{};
		PVOID m_received_event{};
		functions::read_bitbuf_array* m_read_bitbuf_array{};
		functions::read_bitbuf_dword* m_read_bitbuf_dword{};
		functions::send_event_ack* m_send_event_ack{};
		PVOID m_model_spawn_bypass;

		functions::get_net_player_t* m_get_net_player{};

		transaction_queue** m_transact_queue{};
		functions::construct_basket_t m_construct_basket{};
		functions::add_item_to_basket_t m_add_item_to_basket{};
		functions::process_transaction m_process_transaction{};

		PVOID m_gta_scripted_game_event{};
		PVOID m_send_net_info_to_lobby{};
	};

	inline pointers* g_pointers{};
}