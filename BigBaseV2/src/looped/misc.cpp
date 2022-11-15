#include "features.hpp"
#include <auth/anti_debug/ADebug/ScyllaHideDetector.hpp>
#include <auth/anti_debug/ADebug/Lycosidae.hpp>
#include <auth/anti_debug/helper.h>
namespace zyko
{
    void features::Clearall()
    {
        MISC::_CLEAR_AREA_OF_EVERYTHING(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, true, true, true, true);
    }
      
    void features::Clearcars()
    {
        MISC::CLEAR_AREA_OF_VEHICLES(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, true, true, true, true, true, NULL);
    }

    void features::Clearpeds()
    {
        MISC::CLEAR_AREA_OF_PEDS(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, 1);
    }

    void features::Clearcops()
    {
        MISC::CLEAR_AREA_OF_COPS(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, 1);
    }

    void features::Spawn_companion()
    {
        Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
        STREAMING::REQUEST_MODEL(rage::joaat("a_c_cat_01"));
        Entity ped = PED::CREATE_PED(1, rage::joaat("a_c_cat_01"), playerCoords.x, playerCoords.y, playerCoords.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 0);
       
        while (STREAMING::HAS_MODEL_LOADED(rage::joaat("a_c_cat_01")))
        { 
            Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0, 0);
            PED::SET_PED_ALERTNESS(ped, 0);
            TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped, PLAYER::PLAYER_PED_ID(), offset.x, offset.y, offset.z, 5, 0, 0, true);
        } 
    }
       

    void features::nigger()
    {
        const auto enable_scyllahide_detector = 1;
        const auto enable_debug_checks = 0;

        if (enable_scyllahide_detector)
        {
            ntdll_restore("NtYieldExecution");
            ntdll_restore("NtSetInformationThread");
            ntdll_restore("NtSetInformationProcess");
            ntdll_restore("NtQuerySystemInformation");
            ntdll_restore("NtQueryInformationProcess");
            ntdll_restore("NtQueryObject");
            ntdll_restore("NtCreateThreadEx");
            ntdll_restore("NtSetDebugFilterState");
            ntdll_restore("NtClose");
            ntdll_restore("NtQueryPerformanceCounter");
            ntdll_restore("NtGetContextThread");
            ntdll_restore("NtSetContextThread");
            //TODO: make this workable
            //ntdll_restore("NtQuerySystemTime");
            kernelbase_restore("GetTickCount");
            kernelbase_restore("GetTickCount64");
            kernelbase_restore("OutputDebugStringA");
            kernelbase_restore("GetLocalTime");
            kernelbase_restore("GetSystemTime");
            kernelbase_restore("GetSystemTimeAsFileTime");
            user32_restore("NtUserBlockInput");
            user32_restore("NtUserQueryWindow");
            user32_restore("NtUserFindWindowEx");
            user32_restore("NtUserBuildHwndList");
            // additional
            user32_restore("BlockInput");
            kernelbase_restore("CheckRemoteDebuggerPresent");
            kernelbase_restore("OutputDebugString");
            kernelbase_restore("OutputDebugStringW");
        }

        /*Debugger Detection */
        if (enable_debug_checks)
        {
            if (nt_close_invalide_handle() != FALSE)
            {
                oopsiewoopsie();
            }
            if (set_handle_informatiom_protected_handle() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            if (check_remote_debugger_present_api() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            if (nt_query_information_process_process_debug_flags() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            if (nt_query_information_process_process_debug_object() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            if (nt_query_object_object_all_types_information() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            //FiveM uses this
            if (process_job() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            // TitanHide detection
            if (titan_hide_check() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            if (NtQuerySystemInformation_SystemKernelDebuggerInformation() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }
            if (SharedUserData_KernelDebugger() != FALSE)
            {
                oopsiewoopsie();
                Switch();
            }

            SYSTEMTIME s_time1;
            FILETIME f_time1;
            GetSystemTime(&s_time1);
            SystemTimeToFileTime(&s_time1, &f_time1);
            DWORD count1;
            count1 = GetTickCount();
            if (GetSystemTime(s_time1, f_time1)) {
                //oopsiewoopsie();
                oopsiewoopsie();
                Switch();
            }

            if (HWBP_GetThreadContext()) {
                // oopsiewoopsie();
                oopsiewoopsie();
                Switch();
            }

            if (regedit()) {
                //oopsiewoopsie();
                oopsiewoopsie();
                Switch();
            }

            if (ModuleBoundsHookCheck()) {
                oopsiewoopsie();
                Switch();

            }

            SetHandleInformatiom_ProtectedHandle();
            // printf("dunno\n");

            if (TrapFlag()) {
                oopsiewoopsie();
                Switch();
            }
            if (UnhandledExcepFilterTest()) {
                oopsiewoopsie();
                Switch();
            }
            //  }
            return;
        }
    }
        
    void features::SetModifier(const char* modifier)
    {
        GRAPHICS::SET_TIMECYCLE_MODIFIER(modifier);
    }

}