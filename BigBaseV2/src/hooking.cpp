#include "common.hpp"
#include "function_types.hpp"
#include "logger.hpp"
#include "gta/array.hpp"
#include "gta/player.hpp"
#include "gta/script_thread.hpp"
#include "gui.hpp"
#include "hooking.hpp"
#include "memory/module.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "script_mgr.hpp"
#include "features.hpp"
#include "helpers/player_info.h"
#include <MinHook.h>
#include "fiber_pool.hpp"
#include "CNetGamePlayer.hpp"

namespace big
{
	static GtaThread* find_script_thread(rage::joaat_t hash)
	{
		for (auto thread : *g_pointers->m_script_threads)
		{
			if (thread
				&& thread->m_context.m_thread_id
				&& thread->m_handler
				&& thread->m_script_hash == hash)
			{
				return thread;
			}
		}

		return nullptr;
	}

	hooking::hooking() :
		m_swapchain_hook(*g_pointers->m_swapchain, hooks::swapchain_num_funcs),
		m_set_cursor_pos_hook("SetCursorPos", memory::module("user32.dll").get_export("SetCursorPos").as<void*>(), &hooks::set_cursor_pos),

		m_run_script_threads_hook("Script hook", g_pointers->m_run_script_threads, &hooks::run_script_threads),
		m_convert_thread_to_fiber_hook("ConvertThreadToFiber", memory::module("kernel32.dll").get_export("ConvertThreadToFiber").as<void*>(), &hooks::convert_thread_to_fiber),
		m_received_event("RE", g_pointers->m_received_event, &hooks::received_event),
		m_scripted_game_event_hook("SE", g_pointers->m_gta_scripted_game_event, &hooks::scripted_game_event),
		m_send_net_info_to_lobby("SNITL", g_pointers->m_send_net_info_to_lobby, &hooks::send_net_info_to_lobby)
	{
		m_swapchain_hook.hook(hooks::swapchain_present_index, &hooks::swapchain_present);
		m_swapchain_hook.hook(hooks::swapchain_resizebuffers_index, &hooks::swapchain_resizebuffers);

		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		m_send_net_info_to_lobby.enable();
		m_scripted_game_event_hook.enable();
		m_received_event.enable();
		m_swapchain_hook.enable();
		m_og_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&hooks::wndproc)));
		m_set_cursor_pos_hook.enable();

		m_run_script_threads_hook.enable();
		m_convert_thread_to_fiber_hook.enable();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		m_convert_thread_to_fiber_hook.disable();
		m_run_script_threads_hook.disable();

		m_set_cursor_pos_hook.disable();
		SetWindowLongPtrW(g_pointers->m_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_og_wndproc));
		m_swapchain_hook.disable();
		m_received_event.disable();
		m_scripted_game_event_hook.disable();
		m_send_net_info_to_lobby.disable();
	}

	minhook_keepalive::minhook_keepalive()
	{
		MH_Initialize();
	}

	minhook_keepalive::~minhook_keepalive()
	{
		MH_Uninitialize();
	}

	bool hooks::run_script_threads(std::uint32_t ops_to_execute)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_script_mgr.tick();
			}

			return g_hooking->m_run_script_threads_hook.get_original<functions::run_script_threads_t>()(ops_to_execute);
		} EXCEPT_CLAUSE
			return false;
	}

	void* hooks::convert_thread_to_fiber(void* param)
	{
		TRY_CLAUSE
		{
			if (IsThreadAFiber())
			{
				return GetCurrentFiber();
			}

			return g_hooking->m_convert_thread_to_fiber_hook.get_original<decltype(&convert_thread_to_fiber)>()(param);
		} EXCEPT_CLAUSE
			return nullptr;
	}

	HRESULT hooks::swapchain_present(IDXGISwapChain* this_, UINT sync_interval, UINT flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->on_present();
			}

			return g_hooking->m_swapchain_hook.get_original<decltype(&swapchain_present)>(swapchain_present_index)(this_, sync_interval, flags);
		} EXCEPT_CLAUSE
			return NULL;
	}

	HRESULT hooks::swapchain_resizebuffers(IDXGISwapChain* this_, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swapchain_flags)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->pre_reset();

				auto result = g_hooking->m_swapchain_hook.get_original<decltype(&swapchain_resizebuffers)>(swapchain_resizebuffers_index)
					(this_, buffer_count, width, height, new_format, swapchain_flags);

				if (SUCCEEDED(result))
				{
					g_renderer->post_reset();
				}

				return result;
			}

			return g_hooking->m_swapchain_hook.get_original<decltype(&swapchain_resizebuffers)>(swapchain_resizebuffers_index)
				(this_, buffer_count, width, height, new_format, swapchain_flags);
		} EXCEPT_CLAUSE
			return NULL;
	}

	LRESULT hooks::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		TRY_CLAUSE
		{
			if (g_running)
			{
				g_renderer->wndproc(hwnd, msg, wparam, lparam);
			}

			return CallWindowProcW(g_hooking->m_og_wndproc, hwnd, msg, wparam, lparam);
		} EXCEPT_CLAUSE
			return NULL;
	}

	BOOL hooks::set_cursor_pos(int x, int y)
	{
		TRY_CLAUSE
		{
			if (g_gui.m_opened)
				return true;

			return g_hooking->m_set_cursor_pos_hook.get_original<decltype(&set_cursor_pos)>()(x, y);
		} EXCEPT_CLAUSE
			return FALSE;
	}

	bool hooks::received_event(rage::netEventMgr* event_manager, CNetGamePlayer* source_player, CNetGamePlayer* target_player, uint16_t event_id, int event_index, int event_handled_bitset, int unk, rage::datBitBuffer* buffer)
	{
		TRY_CLAUSE
		{
			if (*g_pointers->m_is_session_started && features::g_received_event)
			{
				//auto buffer = std::make_unique<rage::datBitBuffer>((void*)bit_buffer, (uint32_t)bit_buffer_size);

				if (event_id >= 91)
					return false;

				const char* event_name = *(char**)((DWORD64)event_manager + 8i64 * event_id + 243376);
				if (!event_name || !source_player || 4000 < 0 || 4000 >= 32)
				{
					/*g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);*/
					return false;
				}

				switch (event_id)
				{
					case (int)RockstarEvent::NETWORK_CLEAR_PED_TASKS_EVENT:
					{
						if (features::no_freeze_event)
						{
							if (!g_player_info.is_cutscene_playing() && !g_player_info.network_is_activity_session())
							{
							//4000
							//persist_modder::save(4000, 2, event_name);
							g_fiber_pool->queue_job([=] { features::notify_protections("Received Event", event_name, 4000); });
							g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
							return false;
							}
						
						break;
					}	
					}
					
					case (int)RockstarEvent::REPORT_CASH_SPAWN_EVENT:
					{
					if (features::no_pickup_event)
					{

						
						//persist_modder::save(4000, 2, event_name);
						LOG(HACKER) << xorstr_("Detected Cash drop from: ") << source_player->get_name();
						g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Received Event"), event_name, 4000); });
						break;
					}
					}
					case (int)RockstarEvent::NETWORK_CHECK_CODE_CRCS_EVENT:
					
					case (int)RockstarEvent::REPORT_MYSELF_EVENT:
					{
						if(features::no_report_event)
						//persist_modder::save(4000, 3, event_name);
						break;
					}
					case (int)RockstarEvent::REQUEST_CONTROL_EVENT:
					{
						if(features::no_freeze_event)
						/*if (g_player_info.is_in_vehicle())*/
						{
							g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
							return false;
						}
						break;
					}
					case (int)RockstarEvent::GAME_CLOCK_EVENT:
					{
					if (features::no_weather_event)
					{						
						//persist_modder::save(4000, 3, event_name);
						g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Received Event"), event_name, 4000); });
						g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
						return false;
					}
					}
					case (int)RockstarEvent::GAME_WEATHER_EVENT:
					{
						if (features::no_weather_event)
						{
							//persist_modder::save(4000, 3, event_name);
							g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Received Event"), event_name, 4000); });
							g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
							return false;
						}
					}
					case (int)RockstarEvent::KICK_VOTES_EVENT:
					{
						if (features::no_votekick_event)
						{						
						g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Received Event"), event_name, 4000); });
						LOG(RAW_GREEN_TO_CONSOLE) << source_player->get_name() << xorstr_(" voted to kick the: ") << target_player->get_name();
						g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
						return false;
						}
					}
					case (int)RockstarEvent::REMOVE_WEAPON_EVENT:
					{
						if (features::no_weapons_event)
						{						
						//persist_modder::save(4000, 2, event_name);
						g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Received Event"), event_name, 4000); });					
						g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
						return false;
						}
					}
					case (int)RockstarEvent::GIVE_WEAPON_EVENT:
					{
						if (features::no_weapons_event)
						{
							//persist_modder::save(4000, 2, event_name);
							g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Received Event"), event_name, 4000); });
							g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
							return false;
						}
					}
					case (int)RockstarEvent::EXPLOSION_EVENT:
					{
						
						if (features::g_explosion_event)
						{
							g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
							return false;
						}
						break;
					}
					case (int)RockstarEvent::NETWORK_PTFX_EVENT:
					{
						if (features::g_ptfx_event)
						{
							//persist_modder::save(4000, 1, event_name);
							g_pointers->m_send_event_ack(event_manager, source_player, target_player, event_index, event_handled_bitset);
							return false;
						}
						break;
					}
				}
				if (features::g_log_net_event_data)
					LOG(EVENT) << xorstr_("Network Event from: ") << source_player->get_name() << xorstr_(" Type: ") << event_name;
			}

			return g_hooking->m_received_event.get_original<decltype(&received_event)>()(event_manager, source_player, target_player, event_id, event_index, event_handled_bitset, unk, buffer);
		} EXCEPT_CLAUSE

			return false;
	}

	bool hooks::scripted_game_event(CScriptedGameEvent* net_event, CNetGamePlayer* sender)
	{
		TRY_CLAUSE
		{
			if (*g_pointers->m_is_session_started)
			{
				auto args = net_event->m_args;
				auto player_id = g_player_info.player_id;
				auto hash = args[0];
				

				if (features::g_scripted_game_event)
					return true;

				if (net_event)
				{
					if (args[1] == player_id)
					{
						switch (hash)
						{
							if (features::g_crash_protex)
							{
						case 844746317: case 1228916411: //put your hashes like that
							LOG(RAW_GREEN_TO_CONSOLE) << xorstr_("Blocked crash from: ") << sender->get_name();
	/*Crashes*/				g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Blocked crash"), sender->get_name(), 2000); });
							return true;
							}
						
							if (features::g_kick_protex)
							{
						case -1386010354: case 1757755807: case -393294520: case -371781708: case 1514515570:  case 911179316: case 846342319: case 2085853000: case -1970125962: case -1013679841: case -1767058336: case -1892343528: case 1494472464: case 296518236: case 998716537://put your hashes like that
							LOG(RAW_GREEN_TO_CONSOLE) << xorstr_("Blocked crash from: ") << sender->get_name();
	/*Kicks*/				g_fiber_pool->queue_job([=] { features::notify_protections(xorstr_("Blocked crash"), sender->get_name(), 2000); });
							return true;
							}
						


						
						default:
							break;
						
						
						}
					}
				}
			}

			return g_hooking->m_scripted_game_event_hook.get_original<decltype(&scripted_game_event)>()(net_event, sender);
		}
		EXCEPT_CLAUSE

		return false;
	}

	bool hooks::send_net_info_to_lobby(rage::netPlayerData* player, int64_t a2, int64_t a3, DWORD* a4)
	{
		TRY_CLAUSE
		{
			// Local player
			if (features::g_spoof_username)
					memcpy(player->m_name, features::g_username.c_str(), sizeof(player->m_name));

				if (features::g_spoof_ip)
				{
					player->m_external_ip.m_field1 = features::g_ip_address[0];
					player->m_external_ip.m_field2 = features::g_ip_address[1];
					player->m_external_ip.m_field3 = features::g_ip_address[2];
					player->m_external_ip.m_field4 = features::g_ip_address[3];
				}

				if (features::g_spoof_rockstar_id)
				{
					player->m_rockstar_id = features::g_rockstar_id;
					player->m_rockstar_id2 = features::g_rockstar_id;
				}

			return g_hooking->m_send_net_info_to_lobby.get_original<decltype(&send_net_info_to_lobby)>()(player, a2, a3, a4);
		} 
		EXCEPT_CLAUSE

		return false;
	}

}
