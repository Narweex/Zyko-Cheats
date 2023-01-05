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
        MISC::CLEAR_AREA_OF_VEHICLES(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).x, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).y, ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID()), true).z, features::clearradius, true, true, true, true, true, true);
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
       
        
            Vector3 offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0, 0, 0);
            PED::SET_PED_ALERTNESS(ped, 0);
            TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(ped, PLAYER::PLAYER_PED_ID(), offset.x, offset.y, offset.z, 5, 0, 0, true);
       
    }
       

    void features::nigger()
    {
        

        const auto enable_scyllahide_detector = 1;
        const auto enable_debug_checks = 1;

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
            if (IsDebuggerPresent != 0)
            {
                Notify("Debugger", "nigger", 7000, 1);
                
            }
            if (nt_close_invalide_handle() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (set_handle_informatiom_protected_handle() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (check_remote_debugger_present_api() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (nt_query_information_process_process_debug_flags() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (nt_query_information_process_process_debug_object() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (nt_query_object_object_all_types_information() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

               
            }
            //FiveM uses this
            if (process_job() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            // TitanHide detection
            if (titan_hide_check() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

               
            }
            if (NtQuerySystemInformation_SystemKernelDebuggerInformation() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (SharedUserData_KernelDebugger() != FALSE)
            {
                Notify("Detection", "False detection", 7000, 1);

            }

            SYSTEMTIME s_time1;
            FILETIME f_time1;
            GetSystemTime(&s_time1);
            SystemTimeToFileTime(&s_time1, &f_time1);
            DWORD count1;
            count1 = GetTickCount();
            if (GetSystemTime(s_time1, f_time1)) {
                Notify("Detection", "False detection", 7000, 1);

            }

            if (HWBP_GetThreadContext()) {
                Notify("Detection", "False detection", 7000, 1);

            }

            if (regedit()) {
                Notify("Detection", "False detection", 7000, 1);

            }

            if (ModuleBoundsHookCheck()) {
                Notify("Detection", "False detection", 7000, 1);


            }

            SetHandleInformatiom_ProtectedHandle();
            // printf("dunno\n");

            if (TrapFlag()) {
                Notify("Detection", "False detection", 7000, 1);

            }
            if (UnhandledExcepFilterTest()) {
                Notify("Detection", "False detection", 7000, 1);
            }
            //  }
            return;
        }
    }
        
    void features::SetModifier(const char* modifier)
    {
        GRAPHICS::SET_TIMECYCLE_MODIFIER(modifier);
    }

    void features::changeweather(const char* weather)
    {
        MISC::SET_OVERRIDE_WEATHER(weather);
    }

    void features::PlayScenario(const char* name)
    {   
        TASK::TASK_START_SCENARIO_IN_PLACE(PLAYER::PLAYER_PED_ID(), name, 0, true);
    }

    void features::Unfreeze()
    {
        TASK::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
        ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
    }

    void features::SetTimecycleModifier(const char* modifier)
    {
        GRAPHICS::SET_TIMECYCLE_MODIFIER(modifier);
    }

    void features::SaveConfig()
    {
        if (!std::filesystem::exists("C:\\Zyko\\Config\\"))
        {
            std::filesystem::create_directory("C:\\Zyko\\Config\\");
            LOG(INFO_TO_FILE) << "Config Directory didnt exist. Created it";
        }
        nlohmann::json config;
        const char* file_path = "C:\\Zyko\\Config\\saved_config.json";

        std::ofstream config_file(file_path);

        /*config["Styling"] = {
            {"Xpos", g_UiManager->m_PosX}

        };*/
        config["posX"] = g_UiManager->m_PosX;
        config_file << config;
        config_file.close();

        if (config != NULL)
        {
            LOG(INFO_TO_FILE) << "Config Saved";
            Notify("Config Saved", "Make sure to rename it in folder", 7000, 1);
        }
    }

    void features::LoadConfig()
    {
        const char* file_path = "C:\\Zyko\\Config\\saved_config.json";
        std::ifstream config_file(file_path);
        nlohmann::json config = nlohmann::json::parse(config_file);
        config_file >> config;
        LOG(INFO) << config["posX"];
      
      

        
       // g_UiManager->m_PosX = config.at("Xpos");
       
        
      
        config_file.close();

     
    }
}