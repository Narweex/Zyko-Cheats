#pragma once
#include <iostream>
#include <windows.h>
#include <libloaderapi.h>
#include <winternl.h>
#include <Psapi.h>
#include <xstring>
#include <cassert>

#include <intrin.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")



#pragma warning (disable : 4996)
__forceinline const wchar_t *GetWC_l(const char *c)
{
  const size_t cSize = strlen(c) + 1;
  wchar_t *wc = new wchar_t[cSize];
  mbstowcs(wc, c, cSize);
  return wc;
}

// Super Hide String
#include "../hidestr/hide_str.hpp"
#define NTDLL_ GetWC_l((LPCSTR)PRINT_HIDE_STR("ntdll.dll"))

#define DEBUG_READ_EVENT 0x0001
#define DEBUG_PROCESS_ASSIGN 0x0002
#define DEBUG_SET_INFORMATION 0x0004
#define DEBUG_QUERY_INFORMATION 0x0008
#define DEBUG_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
    DEBUG_READ_EVENT | DEBUG_PROCESS_ASSIGN | DEBUG_SET_INFORMATION | \
    DEBUG_QUERY_INFORMATION)

typedef struct object_type_information
{
  UNICODE_STRING type_name;
  ULONG total_number_of_handles;
  ULONG total_number_of_objects;
} object_type_information, *pobject_type_information;

typedef struct object_all_information
{
  ULONG number_of_objects;
  object_type_information object_type_information[1];
} object_all_information, *pobject_all_information;

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION
{
  BOOLEAN KernelDebuggerEnabled;
  BOOLEAN KernelDebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

typedef NTSTATUS(NTAPI *p_nt_close)(HANDLE);
typedef NTSTATUS(NTAPI *p_nt_query_information_process)(IN HANDLE, IN UINT, OUT PVOID, IN ULONG, OUT PULONG);
typedef NTSTATUS(WINAPI *p_nt_query_object)(IN HANDLE, IN UINT, OUT PVOID, IN ULONG, OUT PULONG);
typedef NTSTATUS(__stdcall *t_nt_query_system_information)(IN ULONG, OUT PVOID, IN ULONG, OUT PULONG);

__forceinline BOOL check_remote_debugger_present_api()
{
  auto b_is_dbg_present = FALSE;
  hash_CheckRemoteDebuggerPresent(hash_GetCurrentProcess(), &b_is_dbg_present);
  return b_is_dbg_present;
}

void nt_close_invalide_handle_helper()
{
  const auto nt_close = reinterpret_cast<p_nt_close>(hash_GetProcAddress(hash_GetModuleHandleW(NTDLL_), (LPCSTR)PRINT_HIDE_STR("NtClose")));
  nt_close(reinterpret_cast<HANDLE>(0x99999999ULL));
}

BOOL nt_close_invalide_handle()
{
  __try
  {
    nt_close_invalide_handle_helper();
  }
  __except (EXCEPTION_EXECUTE_HANDLER)
  {
    return TRUE;
  }
  return FALSE;
}

__forceinline BOOL nt_query_information_process_process_debug_flags()
{
  const auto process_debug_flags = 0x1f;
  const auto nt_query_info_process = reinterpret_cast<p_nt_query_information_process>(hash_GetProcAddress(
                                       hash_GetModuleHandleW(NTDLL_), (LPCSTR)PRINT_HIDE_STR("NtQueryInformationProcess")));
  unsigned long no_debug_inherit = 0;
  const auto status = nt_query_info_process(hash_GetCurrentProcess(), process_debug_flags, &no_debug_inherit, sizeof(DWORD),
                      nullptr);
  if (status == 0x00000000 && no_debug_inherit == 0)
    return TRUE;
  return FALSE;
}

__forceinline BOOL nt_query_information_process_process_debug_object()
{
  const auto process_debug_object_handle = 0x1e;
  const auto nt_query_info_process = reinterpret_cast<p_nt_query_information_process>(hash_GetProcAddress(
                                       hash_GetModuleHandleW(NTDLL_), (LPCSTR)PRINT_HIDE_STR("NtQueryInformationProcess")));
  HANDLE h_debug_object = nullptr;
  const unsigned long d_process_information_length = sizeof(ULONG) * 2;
  const auto status = nt_query_info_process(hash_GetCurrentProcess(), process_debug_object_handle, &h_debug_object,
                      d_process_information_length,
                      nullptr);
  if (status == 0x00000000 && h_debug_object)
    return TRUE;
  return FALSE;
}

__forceinline int str_cmp(const wchar_t *x, const wchar_t *y)
{
  while (*x)
  {
    if (*x != *y)
      break;
    x++;
    y++;
  }
  return *static_cast<const wchar_t *>(x) - *static_cast<const wchar_t *>(y);
}

__forceinline BOOL nt_query_object_object_all_types_information()
{
  const auto nt_query_object = reinterpret_cast<p_nt_query_object>(hash_GetProcAddress(
                                 hash_GetModuleHandleW(NTDLL_), (LPCSTR)PRINT_HIDE_STR("NtQueryObject")));
  ULONG size;
  auto status = nt_query_object(nullptr, 3, &size, sizeof(ULONG), &size);
  const auto p_memory = hash_VirtualAlloc(nullptr, (size_t)size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (p_memory == nullptr)
    return FALSE;
  status = nt_query_object(reinterpret_cast<HANDLE>(-1), 3, p_memory, size, nullptr);
  if (status != 0x00000000)
  {
    hash_VirtualFree(p_memory, 0, MEM_RELEASE);
    return FALSE;
  }
  const auto p_object_all_info = static_cast<pobject_all_information>(p_memory);
  auto p_obj_info_location = reinterpret_cast<UCHAR *>(p_object_all_info->object_type_information);
  const auto num_objects = p_object_all_info->number_of_objects;
  for (UINT i = 0; i < num_objects; i++)
  {
    const auto pObjectTypeInfo = reinterpret_cast<pobject_type_information>(p_obj_info_location);
    if (str_cmp((const wchar_t *)(GetWC_l((LPCSTR)PRINT_HIDE_STR("DebugObject"))),
                (const wchar_t *)(pObjectTypeInfo->type_name.Buffer)) == 0)
    {
      if (pObjectTypeInfo->total_number_of_objects > 0)
      {
        hash_VirtualFree(p_memory, 0, MEM_RELEASE);
        return TRUE;
      }
      hash_VirtualFree(p_memory, 0, MEM_RELEASE);
      return FALSE;
    }
    p_obj_info_location = reinterpret_cast<unsigned char *>(pObjectTypeInfo->type_name.Buffer);
    p_obj_info_location += pObjectTypeInfo->type_name.MaximumLength;
    auto tmp = reinterpret_cast<ULONG_PTR>(p_obj_info_location) & -static_cast<int>(sizeof(void *));
    if (static_cast<ULONG_PTR>(tmp) != reinterpret_cast<ULONG_PTR>(p_obj_info_location))
      tmp += sizeof(void *);
    p_obj_info_location = reinterpret_cast<unsigned char *>(tmp);
  }
  hash_VirtualFree(p_memory, 0, MEM_RELEASE);
  return FALSE;
}


BOOL GetSystemTime(SYSTEMTIME s_time1, FILETIME f_time1)
{

    SYSTEMTIME s_time2;
    FILETIME f_time2;
    GetSystemTime(&s_time2);
    SystemTimeToFileTime(&s_time2, &f_time2);
    if ((f_time2.dwLowDateTime - f_time1.dwLowDateTime) / 10000 > 1000) {
        return 1;
    }
    return 0;
}

BOOL GetTickCount(DWORD count1)
{

    DWORD count2;
    count2 = hash_GetTickCount();
    if ((count2 - count1) > 0x10) {
        return 1;
    }
    return 0;
}

BOOL HWBP_GetThreadContext()
{
    BOOL result = FALSE;

    CONTEXT ct;
    ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    hash_GetThreadContext(GetCurrentThread(), &ct);

    if (ct.Dr0 || ct.Dr1 || ct.Dr2 || ct.Dr3)
        result = TRUE;
    return result;
}

//BOOL INT2D_Flag = TRUE;
//void INT2D() {
//    __try {
//        __asm {
//            int 0x2d
//        }
//    }
//    __except (EXCEPTION_EXECUTE_HANDLER) {
//        INT2D_Flag = FALSE;
//    }
//}
//
//BOOL INT3_flag = TRUE;
//void INT3()
//{
//    __try
//    {
//        __asm { int 3 }
//    }
//    __except (EXCEPTION_EXECUTE_HANDLER)
//    {
//        INT3_flag = FALSE;
//    }
//}

BOOL regedit()
{
    BOOL result = FALSE;
    HKEY hKey;
    char lpData[256];
    DWORD lpSize = sizeof(lpData);

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        (LPCWSTR)PRINT_HIDE_STR("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug"),
        0, KEY_QUERY_VALUE, &hKey);
    RegQueryValueEx(hKey, (LPCWSTR)PRINT_HIDE_STR("Debugger"), NULL, NULL, (LPBYTE)&lpData, &lpSize);
    strlwr(lpData);

    if (strstr(lpData, (LPCSTR)PRINT_HIDE_STR("ollydbg")) || strstr(lpData, (LPCSTR)PRINT_HIDE_STR("windbg")) ||
        strstr(lpData, (LPCSTR)PRINT_HIDE_STR("ImmunityDebugger")) || strstr(lpData, (LPCSTR)PRINT_HIDE_STR("ida")))
        result = TRUE;
    return result;
}

//BOOL rdtsc_diff_vmexit()
//{
//    ULONGLONG tsc1 = 0;
//    ULONGLONG tsc2 = 0;
//    ULONGLONG avg = 0;
//    INT cpuInfo[4] = {};
//
//    // Try this 10 times in case of small fluctuations
//    for (INT i = 0; i < 10; i++)
//    {
//        tsc1 = __rdtsc();
//        __cpuid(cpuInfo, 0);
//        tsc2 = __rdtsc();
//
//        // Get the delta of the two RDTSC
//        avg += (tsc2 - tsc1);
//    }
//
//    // We repeated the process 10 times so we make sure our check is as much reliable as we can
//    avg = avg / 10;
//    return (avg < 1000 && avg > 0) ? FALSE : TRUE;
//}


#ifdef _MSC_VER
#pragma once
#endif  // _MSC_VER

#pragma region Application Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#include <specstrings.h>    // for _In_, etc.

#if !defined(__midl) && !defined(SORTPP_PASS)

#if (NTDDI_VERSION >= NTDDI_WINXP)

#ifdef __cplusplus

#define VERSIONHELPERAPI inline bool

#else  // __cplusplus

#define VERSIONHELPERAPI FORCEINLINE BOOL

#endif // __cplusplus

VERSIONHELPERAPI
IsWindowsVersionOrGreater(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
    OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
    DWORDLONG        const dwlConditionMask = VerSetConditionMask(
        VerSetConditionMask(
            VerSetConditionMask(
                0, VER_MAJORVERSION, VER_GREATER_EQUAL),
            VER_MINORVERSION, VER_GREATER_EQUAL),
        VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

    osvi.dwMajorVersion = wMajorVersion;
    osvi.dwMinorVersion = wMinorVersion;
    osvi.wServicePackMajor = wServicePackMajor;

    return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

VERSIONHELPERAPI
IsWindowsVersionOrLesser(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
    OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, { 0 }, 0, 0 };
    DWORDLONG        const dwlConditionMask = VerSetConditionMask(
        VerSetConditionMask(
            0, VER_MAJORVERSION, VER_EQUAL),
        VER_MINORVERSION, VER_LESS_EQUAL);

    osvi.dwMajorVersion = wMajorVersion;
    osvi.dwMinorVersion = wMinorVersion;
    osvi.wServicePackMajor = wServicePackMajor;

    return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask) != FALSE;
}

VERSIONHELPERAPI
IsWindowsXPOrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0);
}

VERSIONHELPERAPI
IsWindowsXPSP1OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 1);
}

VERSIONHELPERAPI
IsWindowsXPSP2OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 2);
}

VERSIONHELPERAPI
IsWindowsXPSP3OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 3);
}

VERSIONHELPERAPI
IsWindowsVistaOrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
}

VERSIONHELPERAPI
IsWindowsVistaSP1OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 1);
}

VERSIONHELPERAPI
IsWindowsVistaSP2OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 2);
}

VERSIONHELPERAPI
IsWindows7OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
}

VERSIONHELPERAPI
IsWindows7SP1OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 1);
}

VERSIONHELPERAPI
IsWindows8OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
}

VERSIONHELPERAPI
IsWindows8Point1OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINBLUE), LOBYTE(_WIN32_WINNT_WINBLUE), 0);
}

VERSIONHELPERAPI
IsWindows10OrGreater()
{
    return IsWindowsVersionOrGreater(10, 0, 0);
}

VERSIONHELPERAPI
IsWindowsServer()
{
    OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION };
    DWORDLONG        const dwlConditionMask = VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);

    return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
}



VERSIONHELPERAPI
IsWindowsXPOr2k()
{
    return IsWindowsVersionOrLesser(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0);
}


#endif // NTDDI_VERSION

#endif // defined(__midl)

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */

enum API_IDENTIFIER
{
    API_CsrGetProcessId,
    API_EnumSystemFirmwareTables,
    API_GetSystemFirmwareTable,
    API_GetNativeSystemInfo,
    API_GetProductInfo,
    API_EnumProcessModulesEx_Kernel,
    API_EnumProcessModulesEx_PSAPI,
    API_IsWow64Process,
    API_LdrEnumerateLoadedModules,
    API_NtClose,
    API_NtCreateDebugObject,
    API_NtDelayExecution,
    API_NtQueryInformationThread,
    API_NtQueryInformationProcess,
    API_NtQueryLicenseValue,
    API_NtQueryObject,
    API_NtQuerySystemInformation,
    API_NtSetInformationThread,
    API_NtWow64QueryInformationProcess64,
    API_NtWow64QueryVirtualMemory64,
    API_NtWow64ReadVirtualMemory64,
    API_NtYieldExecution,
    API_RtlGetVersion,
    API_RtlInitUnicodeString,
    API_WudfIsAnyDebuggerPresent,
    API_WudfIsKernelDebuggerPresent,
    API_WudfIsUserDebuggerPresent,
};

enum API_OS_VERSION
{
    NONE,
    WIN_XP,
    WIN_XP_SP1,
    WIN_XP_SP2,
    WIN_XP_SP3,
    WIN_VISTA,
    WIN_VISTA_SP1,
    WIN_VISTA_SP2,
    WIN_7,
    WIN_7_SP1,
    WIN_80,
    WIN_81,
    WIN_10,
    VERSION_MAX
};

enum API_OS_BITS
{
    ANY,
    X86_ONLY,
    X64_ONLY,
};

struct VERSION_FUNCTION_MAP
{
    API_OS_VERSION Version;
    bool(*Function)();

    VERSION_FUNCTION_MAP(API_OS_VERSION version, bool(*function)())
    {
        Version = version;
        Function = function;
    }

    VERSION_FUNCTION_MAP()
    {
    }
};

struct API_DATA
{
    API_IDENTIFIER Identifier;
    const char* Library;
    const char* EntryName;
    API_OS_BITS PlatformBits;
    API_OS_VERSION MinVersion;
    API_OS_VERSION RemovedInVersion;
    bool Available;
    bool ExpectedAvailable;
    void* Pointer;

    API_DATA(API_IDENTIFIER identifier, const char* lib, const char* name, API_OS_BITS bits, API_OS_VERSION minVersion, API_OS_VERSION removedInVersion)
    {
        Identifier = identifier;
        Library = lib;
        EntryName = name;
        PlatformBits = bits;
        MinVersion = minVersion;
        RemovedInVersion = removedInVersion;
        Available = false;
        ExpectedAvailable = false;
        Pointer = nullptr;
    }
};

const VERSION_FUNCTION_MAP VersionFunctionMap[] = {
    { API_OS_VERSION::NONE, nullptr },
    { API_OS_VERSION::WIN_XP, IsWindowsXPOrGreater },
    { API_OS_VERSION::WIN_XP_SP1, IsWindowsXPSP1OrGreater },
    { API_OS_VERSION::WIN_XP_SP2, IsWindowsXPSP2OrGreater },
    { API_OS_VERSION::WIN_XP_SP3, IsWindowsXPSP3OrGreater },
    { API_OS_VERSION::WIN_VISTA, IsWindowsVistaOrGreater },
    { API_OS_VERSION::WIN_VISTA_SP1, IsWindowsVistaSP1OrGreater },
    { API_OS_VERSION::WIN_VISTA_SP2, IsWindowsVistaSP2OrGreater },
    { API_OS_VERSION::WIN_7, IsWindows7OrGreater },
    { API_OS_VERSION::WIN_7_SP1, IsWindows7SP1OrGreater },
    { API_OS_VERSION::WIN_80, IsWindows8OrGreater },
    { API_OS_VERSION::WIN_81, IsWindows8Point1OrGreater },
    { API_OS_VERSION::WIN_10, IsWindows10OrGreater },
};
#define API_COUNT (sizeof(ApiData)/sizeof(*ApiData))
API_DATA ApiData[] = {
    /*                Identifier                            Library             Export Name                         X86/X64/either			Minimum OS Version              Removed in OS Version   */
    { API_IDENTIFIER::API_CsrGetProcessId,				    "ntdll.dll",		"CsrGetProcessId",				    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    /* the EnumProcessModulesEx API was moved from psapi.dll into kernel32.dll for Windows 7, then back out afterwards, so we need both versions. */
    { API_IDENTIFIER::API_EnumProcessModulesEx_Kernel,		"kernel32.dll",		"EnumProcessModulesEx",				API_OS_BITS::ANY,		API_OS_VERSION::WIN_7,			API_OS_VERSION::WIN_80 },
    { API_IDENTIFIER::API_EnumProcessModulesEx_PSAPI,		"psapi.dll",		"EnumProcessModulesEx",				API_OS_BITS::ANY,		API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_EnumSystemFirmwareTables,		    "kernel32.dll",		"EnumSystemFirmwareTables",		    API_OS_BITS::ANY,		API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_GetNativeSystemInfo,			    "kernel32.dll",		"GetNativeSystemInfo",		    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_GetProductInfo,				    "kernel32.dll",		"GetProductInfo",				    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_GetSystemFirmwareTable,		    "kernel32.dll",		"GetSystemFirmwareTable",	    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_IsWow64Process,				    "kernel32.dll",		"IsWow64Process",			    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP_SP2,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_LdrEnumerateLoadedModules,	    "ntdll.dll",		"LdrEnumerateLoadedModules",    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP_SP1,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtClose,						    "ntdll.dll",		"NtClose",					    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtCreateDebugObject,			    "ntdll.dll",		"NtCreateDebugObject",		    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtDelayExecution,				    "ntdll.dll",		"NtDelayExecution",			    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtQueryInformationThread,		    "ntdll.dll",		"NtQueryInformationThread",	    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtQueryInformationProcess,	    "ntdll.dll",		"NtQueryInformationProcess",	    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtQueryLicenseValue,			    "ntdll.dll",		"NtQueryLicenseValue",	    		API_OS_BITS::ANY,		API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtQueryObject,				    "ntdll.dll",		"NtQueryObject",				    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtQuerySystemInformation,		    "ntdll.dll",		"NtQuerySystemInformation",		    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtSetInformationThread,		    "ntdll.dll",		"NtSetInformationThread",		    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtWow64QueryInformationProcess64, "ntdll.dll",        "NtWow64QueryInformationProcess64",	API_OS_BITS::X86_ONLY,	API_OS_VERSION::WIN_XP_SP1,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtWow64ReadVirtualMemory64,	    "ntdll.dll",		"NtWow64ReadVirtualMemory64",	    API_OS_BITS::X86_ONLY,	API_OS_VERSION::WIN_XP_SP1,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_NtWow64QueryVirtualMemory64,	    "ntdll.dll",		"NtWow64QueryVirtualMemory64",	    API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP_SP1,		API_OS_VERSION::WIN_10 },
    { API_IDENTIFIER::API_NtYieldExecution,			    	"ntdll.dll",		"NtYieldExecution",			    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_RtlInitUnicodeString,			    "ntdll.dll",		"RtlInitUnicodeString",		    	API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_RtlGetVersion,		    		"ntdll.dll",		"RtlGetVersion",		    		API_OS_BITS::ANY,		API_OS_VERSION::WIN_XP,			API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_WudfIsAnyDebuggerPresent,	    	"WUDFPlatform.dll",	"WudfIsAnyDebuggerPresent",	    	API_OS_BITS::X64_ONLY,	API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_WudfIsKernelDebuggerPresent,	    "WUDFPlatform.dll",	"WudfIsKernelDebuggerPresent",	    API_OS_BITS::X64_ONLY,	API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE },
    { API_IDENTIFIER::API_WudfIsUserDebuggerPresent,	    "WUDFPlatform.dll",	"WudfIsUserDebuggerPresent",    	API_OS_BITS::X64_ONLY,	API_OS_VERSION::WIN_VISTA,		API_OS_VERSION::NONE }
};
void* GetAPI(API_IDENTIFIER api)
{
    for (int i = 0; i < API_COUNT; i++)
    {
        if (ApiData[i].Identifier == api)
        {
            if (ApiData[i].Available)
            {
                return ApiData[i].Pointer;
            }
            else
            {
                return nullptr;
            }
        }
    }
    assert(false);
    return nullptr;
}

bool IsAvailable(API_IDENTIFIER api)
{
    for (int i = 0; i < API_COUNT; i++)
    {
        if (ApiData[i].Identifier == api)
        {
            return ApiData[i].Available;
        }
    }
    assert(false);
    return false;
}

//i'm not cheap, you're cheap!
DWORD GetPidByProcessName2(WCHAR* name) {
    PROCESSENTRY32W entry;
    memset(&entry, 0, sizeof(PROCESSENTRY32W));
    entry.dwSize = sizeof(PROCESSENTRY32W);

    DWORD pid = -1;
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32FirstW(hSnapShot, &entry)) {
        do {
            if (!wcscmp(name, entry.szExeFile)) {
                pid = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapShot, &entry));
    }

    CloseHandle(hSnapShot);

    return pid;
}

BOOL SetDebugPrivileges(VOID) {
    TOKEN_PRIVILEGES priv = { 0 };
    HANDLE hToken = NULL;
    BOOL bResult = FALSE;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        //print_last_error(_T("OpenProcessToken"));
        return bResult;
    }

    priv.PrivilegeCount = 1;
    priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &priv.Privileges[0].Luid)) {

        bResult = AdjustTokenPrivileges(hToken, FALSE, &priv, 0, NULL, NULL);
        if (!bResult) {
            //print_last_error(_T("AdjustTokenPrivileges"));
        }
    }
    else {
        //print_last_error(_T("LookupPrivilegeValue"));
    }

    CloseHandle(hToken);
    return bResult;
}
#include <tchar.h>

typedef struct _PS_ATTRIBUTE {
    ULONG Attribute;
    SIZE_T Size;
    union
    {
        ULONG Value;
        PVOID ValuePtr;
    } u1;
    PSIZE_T ReturnLength;
} PS_ATTRIBUTE, * PPS_ATTRIBUTE;

typedef struct _PS_ATTRIBUTE_LIST {
    SIZE_T TotalLength;
    PS_ATTRIBUTE Attributes[1];
} PS_ATTRIBUTE_LIST, * PPS_ATTRIBUTE_LIST;

typedef NTSTATUS(WINAPI* pNtCreateThreadEx)(
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK DesiredAccess,
    IN OPTIONAL POBJECT_ATTRIBUTES ObjectAttributes,
    IN HANDLE ProcessHandle,
    IN PVOID StartRoutine,
    IN OPTIONAL PVOID Argument,
    IN ULONG CreateFlags, //THREAD_CREATE_FLAGS_*
    IN OPTIONAL ULONG_PTR ZeroBits,
    IN OPTIONAL SIZE_T StackSize,
    IN OPTIONAL SIZE_T MaximumStackSize,
    IN OPTIONAL PPS_ATTRIBUTE_LIST AttributeList
    );

typedef ULONG(NTAPI* pRtlNtStatusToDosError)(IN NTSTATUS Status);

#define PS_ATTRIBUTE_NUMBER_MASK 0x0000ffff
#define PS_ATTRIBUTE_THREAD 0x00010000
#define PS_ATTRIBUTE_INPUT 0x00020000
#define PS_ATTRIBUTE_ADDITIVE 0x00040000


#define PsAttributeValue(Number, Thread, Input, Additive) \
	(((Number) & PS_ATTRIBUTE_NUMBER_MASK) | \
	((Thread) ? PS_ATTRIBUTE_THREAD : 0) | \
	((Input) ? PS_ATTRIBUTE_INPUT : 0) | \
	((Additive) ? PS_ATTRIBUTE_ADDITIVE : 0))

typedef enum _PS_ATTRIBUTE_NUM {
    PsAttributeClientId = 3,
} PS_ATTRIBUTE_NUM;

#define PS_ATTRIBUTE_CLIENT_ID \
    PsAttributeValue(PsAttributeClientId, TRUE, FALSE, FALSE)

BOOL IsDllInjected(DWORD dwProcessId, LPTSTR DllName)
{
    BOOL bFound = FALSE;
    HANDLE hSnapshot;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {

        MODULEENTRY32 me32;
        me32.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(hSnapshot, &me32)) {

            do {

                if (StrCmpI(me32.szModule, DllName) == 0) {
                    bFound = TRUE;
                    break;
                }

            } while (Module32Next(hSnapshot, &me32));

        }

        CloseHandle(hSnapshot);
    }

    return bFound;
}

DWORD GetMainThreadId(DWORD pid)
{
    /* Get main thread id from process id */
    HANDLE h = hash_CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (h != INVALID_HANDLE_VALUE) {
        THREADENTRY32 te;
        te.dwSize = sizeof(te);
        if (hash_Thread32First(h, &te))
        {
            do
            {
                if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID)) {
                    if (te.th32OwnerProcessID == pid) {
                        HANDLE hThread = hash_OpenThread(READ_CONTROL, FALSE, te.th32ThreadID);
                        if (!hThread){}
                            //print_last_error(_T("OpenThread"));
                        else {
                            hash_CloseHandle(hThread);
                            hash_CloseHandle(h);
                            return te.th32ThreadID;
                        }
                    }
                }

            } while (hash_Thread32Next(h, &te));
        }
        hash_CloseHandle(h);
    }

    //print_last_error(_T("CreateToolhelp32Snapshot"));
    return (DWORD)0;
}

typedef DWORD(WINAPI* pRtlCreateUserThread)(
    IN HANDLE 					ProcessHandle,
    IN PSECURITY_DESCRIPTOR 	SecurityDescriptor,
    IN BOOL 					CreateSuspended,
    IN ULONG					StackZeroBits,
    IN OUT PULONG				StackReserved,
    IN OUT PULONG				StackCommit,
    IN LPVOID					StartAddress,
    IN LPVOID					StartParameter,
    OUT HANDLE 					ThreadHandle,
    OUT LPVOID					ClientID
    );


BOOL SetWindowsHooksEx_Injection()
{
    TCHAR lpDllName[] = _T("InjectedDLL.dll");
    TCHAR lpDllPath[MAX_PATH];
    HOOKPROC myFunctionAddress;
    HMODULE hOurDll;
    DWORD dwProcessId, dwThreadId;
    HHOOK hHook;

    /* Get Process ID from Process name */
    dwProcessId = GetPidByProcessName2((WCHAR*)PRINT_HIDE_STR("notepad.exe"));
    if (dwProcessId == NULL)
        return FALSE;
    //_tprintf(_T("\t[+] Getting proc id: %u\n"), dwProcessId);

    /* Get thread id from process id */
    dwThreadId = GetMainThreadId(dwProcessId);
    if (dwThreadId == NULL)
        return FALSE;
    //_tprintf(_T("\t[+] Getting main thread id of proc id: %u\n"), dwThreadId);

    /* Get the full path of the dll to be injected */
    GetFullPathName(lpDllName, MAX_PATH, lpDllPath, NULL);
    //_tprintf(_T("\t[+] Full DLL Path: %s\n"), lpDllPath);

    /* Obtain a handle to our injected dll */
    hOurDll = LoadLibrary(lpDllPath);
    if (hOurDll == NULL) {
        //print_last_error(_T("LoadLibrary"));
        return FALSE;
    }

    /* Get 'MyProc' address */
    //_tprintf(_T("\t[+] Looking for 'MyProc' in our dll\n"));
    myFunctionAddress = (HOOKPROC)hash_GetProcAddress(hOurDll, "MyProc");
    if (myFunctionAddress == NULL) {
        //print_last_error(_T("GetProcAddress"));
        return FALSE;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), myFunctionAddress);

    /* Injection happens here */
    hHook = SetWindowsHookEx(WH_KEYBOARD, myFunctionAddress, hOurDll, dwThreadId);
    if (hHook == NULL) {
        //print_last_error(_T("SetWindowsHookEx"));
        return FALSE;
    }

    /* Unhook */
    //_tprintf(_T("SetWindowsHookEx created successfully ...\n"));

    /* When we want to remove the hook */
    // UnhookWindowsHookEx(hHook);

    return TRUE;

}


BOOL RtlCreateUserThread_Injection()
{
    // some vars
    HMODULE hNtdll;
    DWORD dwProcessId;
    HANDLE hProcess;
    TCHAR lpDllName[] = _T("InjectedDLL.dll");
    TCHAR lpDllPath[MAX_PATH];
    LPVOID lpBaseAddress = NULL;
    BOOL bStatus = FALSE;
    HMODULE hKernel32;
    FARPROC LoadLibraryAddress;
    HANDLE  hRemoteThread = NULL;
    SIZE_T dwSize;
    NTSTATUS Status;

    // we have to import our function
    pRtlCreateUserThread RtlCreateUserThread = NULL;

    /*
    GetLastError cannot be used with RtlCreateUserThread because this routine does not set Win32 LastError value.
    Native status code must be translated to Win32 error code and set manually.
    */
    pRtlNtStatusToDosError RtlNtStatusToDosErrorPtr = NULL;

    /* Get Process ID from Process name */
    dwProcessId = GetPidByProcessName2((WCHAR*)PRINT_HIDE_STR("notepad.exe"));
    if (dwProcessId == NULL)
        return FALSE;
    //_tprintf(_T("\t[+] Getting proc id: %u\n"), dwProcessId);

    /* Obtain a handle the process */
    hProcess = hash_OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (hProcess == NULL) {
        //print_last_error(_T("OpenProcess"));
        return FALSE;
    }

    /* Get module handle of ntdll */
    hNtdll = GetModuleHandle((LPCWSTR)PRINT_HIDE_STR("ntdll.dll"));
    if (hNtdll == NULL) {
       // print_last_error(_T("GetModuleHandle"));
        goto Cleanup;
    }

    /* Get routine pointer, failure is not critical */
    RtlNtStatusToDosErrorPtr = (pRtlNtStatusToDosError)hash_GetProcAddress(hNtdll, (LPCSTR)PRINT_HIDE_STR("RtlNtStatusToDosError"));

    /* Obtain a handle to kernel32 */
    hKernel32 = GetModuleHandle(_T("kernel32.dll"));
    if (hKernel32 == NULL) {
        //print_last_error(_T("GetModuleHandle"));
        goto Cleanup;
    }

    // Get the address RtlCreateUserThread
    //_tprintf(_T("\t[+] Looking for RtlCreateUserThread in ntdll\n"));
    RtlCreateUserThread = (pRtlCreateUserThread)hash_GetProcAddress(hNtdll, (LPCSTR)PRINT_HIDE_STR("RtlCreateUserThread"));
    if (RtlCreateUserThread == NULL) {
        //print_last_error(_T("GetProcAddress"));
        goto Cleanup;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), RtlCreateUserThread);

    /* Get LoadLibrary address */
    //_tprintf(_T("\t[+] Looking for LoadLibrary in kernel32\n"));
    LoadLibraryAddress = hash_GetProcAddress(hKernel32, (LPCSTR)PRINT_HIDE_STR("LoadLibraryW"));
    if (LoadLibraryAddress == NULL) {
        //print_last_error(_T("GetProcAddress"));
        goto Cleanup;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), LoadLibraryAddress);

    /* Get the full path of the dll */
    GetFullPathName(lpDllName, MAX_PATH, lpDllPath, NULL);
    //_tprintf(_T("\t[+] Full DLL Path: %s\n"), lpDllPath);

    /* Calculate the number of bytes needed for the DLL's pathname */
    dwSize = _tcslen(lpDllPath) * sizeof(TCHAR);

    /* Allocate memory into the remote process */
    //_tprintf(_T("\t[+] Allocating space for the path of the DLL\n"));
    lpBaseAddress = hash_VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpBaseAddress == NULL) {
        //print_last_error(_T("VirtualAllocEx"));
        goto Cleanup;
    }

    /* Write to the remote process */
    //printf("\t[+] Writing into the current process space at 0x%p\n", lpBaseAddress);
    if (!hash_WriteProcessMemory(hProcess, lpBaseAddress, lpDllPath, dwSize, NULL)) {
        ////print_last_error(_T("WriteProcessMemory"));
        goto Cleanup;
    }

    /* Create the more thread */
    Status = RtlCreateUserThread(hProcess, NULL, 0, 0, 0, 0, LoadLibraryAddress, lpBaseAddress, &hRemoteThread, NULL);
    if (!NT_SUCCESS(Status)) {
        if (RtlNtStatusToDosErrorPtr) {
            hash_SetLastError(RtlNtStatusToDosErrorPtr(Status));
        }
        else {
            hash_SetLastError(ERROR_INTERNAL_ERROR);
        }
        //print_last_error(_T("RtlCreateUserThread"));
    }
    else {
        //_tprintf(_T("Remote thread has been created successfully ...\n"));
        hash_WaitForSingleObject(hRemoteThread, INFINITE);
        hash_CloseHandle(hRemoteThread);

        /* assign function success return result */
        bStatus = TRUE;
    }
Cleanup:
    /* hProcess is always initialized here. */
    if (lpBaseAddress) {
        hash_VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
    }
    hash_CloseHandle(hProcess);

    return bStatus;
}


BOOL QueueUserAPC_Injection()
{
    TCHAR lpDllName[] = _T("InjectedDLL.dll");
    TCHAR lpDllPath[MAX_PATH];

    HANDLE hThreadSnapshot = INVALID_HANDLE_VALUE;

    DWORD dwTargetProcessId, dwCurrentProcessId = GetCurrentProcessId();

    HANDLE hProcess = NULL;
    HANDLE hThread = NULL;

    HMODULE hKernel32;

    FARPROC LoadLibraryAddress;
    LPVOID lpBaseAddress = NULL;
    BOOL bStatus = FALSE;


    /* Get Process ID from Process name */

    //
    // calc used because it has multiple threads and some of them maybe alertable.
    //
    dwTargetProcessId = GetPidByProcessName2((WCHAR*)PRINT_HIDE_STR("calc.exe"));
    if (dwTargetProcessId == 0)
        dwTargetProcessId = GetPidByProcessName2((WCHAR*)PRINT_HIDE_STR("win32calc.exe"));//w10 classic calc

    if (dwTargetProcessId == 0) {
        //print_last_error(_T("GetProcessIdFromName"));
        return FALSE;
    }

    /* Obtain a hmodule of kernel32 */
    hKernel32 = GetModuleHandle((LPCWSTR)PRINT_HIDE_STR("kernel32.dll"));
    if (hKernel32 == NULL) {
        //print_last_error(_T("GetModuleHandle"));
        return FALSE;
    }

    /* Get LoadLibrary address */
    //_tprintf(_T("\t[+] Looking for LoadLibrary in kernel32\n"));
    LoadLibraryAddress = hash_GetProcAddress(hKernel32, (LPCSTR)PRINT_HIDE_STR("LoadLibraryW"));
    if (LoadLibraryAddress == NULL) {
        //print_last_error(_T("GetProcAddress"));
        return FALSE;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), LoadLibraryAddress);

    //_tprintf(_T("\t[+] Getting proc id: %u\n"), dwTargetProcessId);

    /* Obtain a handle the process */
    hProcess = hash_OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTargetProcessId);
    if (hProcess == NULL) {
        //print_last_error(_T("OpenProcess"));
        return FALSE;
    }

    do { // not a loop

        /* Get the full path of the dll */
        GetFullPathName(lpDllName, MAX_PATH, lpDllPath, NULL);
        //_tprintf(_T("\t[+] Full DLL Path: %s\n"), lpDllPath);

        // The maximum size of the string buffer.
        SIZE_T WriteBufferSize = _tcslen(lpDllPath) * sizeof(TCHAR);

        /* Allocate memory into the remote process */
        //_tprintf(_T("\t[+] Allocating space for the path of the DLL\n"));
        lpBaseAddress = hash_VirtualAllocEx(hProcess, NULL, WriteBufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (lpBaseAddress == NULL) {
            //print_last_error(_T("VirtualAllocEx"));
            break;
        }

        /* Write to the remote process */
        //printf("\t[+] Writing into the current process space at 0x%p\n", lpBaseAddress);
        if (!hash_WriteProcessMemory(hProcess, lpBaseAddress, lpDllPath, WriteBufferSize, NULL)) {
            //print_last_error(_T("WriteProcessMemory"));
            break;
        }

        hThreadSnapshot = hash_CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hThreadSnapshot == INVALID_HANDLE_VALUE)
            break;

        THREADENTRY32 te32;
        te32.dwSize = sizeof(THREADENTRY32);

        //
        // Brute force threads to find suitable alertable thread for APC injection (if any).
        //
        if (hash_Thread32First(hThreadSnapshot, &te32)) {
            do {
                if (te32.th32OwnerProcessID == dwTargetProcessId) {

                    hThread = hash_OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
                    if (hThread) {

                        if (hash_QueueUserAPC((PAPCFUNC)LoadLibraryAddress, hThread, (ULONG_PTR)lpBaseAddress)) {

                            if (IsDllInjected(dwTargetProcessId, lpDllName)) {
                                bStatus = TRUE;
                            }
                        }
                        hash_CloseHandle(hThread);
                    }
                }

                // dll injected - leave
                if (bStatus) {
                    //_tprintf(_T("\t[+] Dll has been injected successfully ...\n"));
                    break;
                }

            } while (hash_Thread32Next(hThreadSnapshot, &te32));
        }

    } while (FALSE); // not a loop

    //
    // Cleanup.
    //
    if (hThreadSnapshot != INVALID_HANDLE_VALUE) hash_CloseHandle(hThreadSnapshot);

    if (lpBaseAddress) {
        hash_VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
    }

    hash_CloseHandle(hProcess);

    return bStatus;
}


BOOL NtCreateThreadEx_Injection()
{
    // some vars
    HMODULE hNtdll;
    DWORD dwProcessId;
    HANDLE hProcess = NULL;
    TCHAR lpDllName[] = _T("InjectedDLL.dll");
    TCHAR lpDllPath[MAX_PATH];
    LPVOID lpBaseAddress = NULL;
    BOOL bStatus = FALSE;
    HMODULE hKernel32;
    FARPROC LoadLibraryAddress;
    HANDLE  hRemoteThread = NULL;
    NTSTATUS Status;
    SIZE_T dwSize;
    CLIENT_ID ClientId;
    PS_ATTRIBUTE_LIST PsAttrList;

    // we have to import our function
    pNtCreateThreadEx NtCreateThreadEx = NULL;

    /*
        GetLastError cannot be used with NtCreateThreadEx because this service does not set Win32 LastError value.
        Native status code must be translated to Win32 error code and set manually.
    */
    pRtlNtStatusToDosError RtlNtStatusToDosErrorPtr = NULL;

    /* Get Process ID from Process name */
    dwProcessId = GetPidByProcessName2((WCHAR*)PRINT_HIDE_STR("notepad.exe"));
    if (dwProcessId == 0)
        return FALSE;
    //_tprintf(_T("\t[+] Getting proc id: %u\n"), dwProcessId);

    /* Obtain a handle the process */
    hProcess = hash_OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (hProcess == NULL) {
        //print_last_error(_T("OpenProcess"));
        goto Cleanup;
    }

    /* Get module handle of ntdll */
    hNtdll = GetModuleHandle((LPCWSTR)PRINT_HIDE_STR("ntdll.dll"));
    if (hNtdll == NULL) {
       // print_last_error(_T("GetModuleHandle"));
        goto Cleanup;
    }

    /* Obtain a handle to kernel32 */
    hKernel32 = GetModuleHandle((LPCWSTR)PRINT_HIDE_STR("kernel32.dll"));
    if (hKernel32 == NULL) {
       // print_last_error(_T("GetModuleHandle"));
        goto Cleanup;
    }

    /* Get routine pointer, failure is not critical */
    RtlNtStatusToDosErrorPtr = (pRtlNtStatusToDosError)hash_GetProcAddress(hNtdll, (LPCSTR)PRINT_HIDE_STR("RtlNtStatusToDosError"));

    // Get the address NtCreateThreadEx
    //_tprintf(_T("\t[+] Looking for NtCreateThreadEx in ntdll\n"));
    NtCreateThreadEx = (pNtCreateThreadEx)hash_GetProcAddress(hNtdll, (LPCSTR)PRINT_HIDE_STR("NtCreateThreadEx"));
    if (NtCreateThreadEx == NULL) {
        //print_last_error(_T("GetProcAddress"));
        goto Cleanup;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), NtCreateThreadEx);

    /* Get LoadLibrary address */
    //_tprintf(_T("\t[+] Looking for LoadLibrary in kernel32\n"));
    LoadLibraryAddress = hash_GetProcAddress(hKernel32, (LPCSTR)PRINT_HIDE_STR("LoadLibraryW"));
    if (LoadLibraryAddress == NULL) {
        //print_last_error(_T("GetProcAddress"));
        goto Cleanup;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), LoadLibraryAddress);

    /* Get the full path of the dll */
    GetFullPathName(lpDllName, MAX_PATH, lpDllPath, NULL);
    //_tprintf(_T("\t[+] Full DLL Path: %s\n"), lpDllPath);

    /* Calculate the number of bytes needed for the DLL's pathname */
    dwSize = _tcslen(lpDllPath) * sizeof(TCHAR);

    /* Allocate memory into the remote process */
    //_tprintf(_T("\t[+] Allocating space for the path of the DLL\n"));
    lpBaseAddress = hash_VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpBaseAddress == NULL) {
        //print_last_error(_T("VirtualAllocEx"));
        goto Cleanup;
    }

    /* Write to the remote process */
    //printf("\t[+] Writing into the current process space at 0x%p\n", lpBaseAddress);
    if (!hash_WriteProcessMemory(hProcess, lpBaseAddress, lpDllPath, dwSize, NULL)) {
        //print_last_error(_T("WriteProcessMemory"));
        goto Cleanup;
    }

    /* Create the more thread */
    SecureZeroMemory(&PsAttrList, sizeof(PsAttrList));

    /* Setup attributes entry */
    PsAttrList.TotalLength = sizeof(PS_ATTRIBUTE_LIST);
    PsAttrList.Attributes[0].Attribute = PS_ATTRIBUTE_CLIENT_ID;
    PsAttrList.Attributes[0].Size = sizeof(CLIENT_ID);
    PsAttrList.Attributes[0].u1.ValuePtr = &ClientId;

    Status = NtCreateThreadEx(&hRemoteThread, THREAD_ALL_ACCESS, NULL, hProcess,
        (LPTHREAD_START_ROUTINE)LoadLibraryAddress, lpBaseAddress, 0, 0, 0, 0, &PsAttrList);

    if (!NT_SUCCESS(Status)) {
        if (RtlNtStatusToDosErrorPtr) {
            hash_SetLastError(RtlNtStatusToDosErrorPtr(Status));
        }
        else {
            hash_SetLastError(ERROR_INTERNAL_ERROR);
        }
        //print_last_error(_T("NtCreateThreadEx"));
    }

    else {
       // _tprintf(_T("Remote thread has been created successfully ...\n"));
        hash_WaitForSingleObject(hRemoteThread, INFINITE);
        hash_CloseHandle(hRemoteThread);

        /* assign function success return result */
        bStatus = TRUE;
    }

Cleanup:
    /* If lpBaseAddress initialized then hProcess is initialized too because of upper check. */
    if (lpBaseAddress) {
        hash_VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
    }
    if (hProcess) hash_CloseHandle(hProcess);

    return bStatus;
}

BOOL CreateRemoteThread_Injection()
{
    /* Some vars */
    DWORD dwProcessId;
    HANDLE hProcess = NULL, hRemoteThread = NULL;
    HMODULE hKernel32;
    FARPROC LoadLibraryAddress;
    LPVOID lpBaseAddress = NULL;
    TCHAR lpDllName[] = L"InjectedDLL.dll";
    TCHAR lpDllPath[MAX_PATH];
    SIZE_T dwSize;
    BOOL bStatus = FALSE, bDebugPrivilegeEnabled;

    /* Get Process ID from Process name */
    dwProcessId = GetPidByProcessName2((WCHAR*)PRINT_HIDE_STR("notepad.exe"));
    if (dwProcessId == NULL)
        return FALSE;
    //_tprintf(_T("\t[+] Getting proc id: %u\n"), dwProcessId);

    /* Set Debug privilege */
    bDebugPrivilegeEnabled = SetDebugPrivileges();
   // _tprintf(_T("\t[+] Setting Debug Privileges [%d]\n"), bDebugPrivilegeEnabled);
    if (!bDebugPrivilegeEnabled)
        return FALSE;

    /* Obtain a handle the process */
    hProcess = hash_OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
    if (hProcess == NULL) {
        //print_last_error(_T("OpenProcess"));
        goto Cleanup;
    }

    /* Obtain a handle to kernel32 */
    hKernel32 = GetModuleHandle((LPCWSTR)PRINT_HIDE_STR("kernel32.dll"));
    if (hKernel32 == NULL) {
        //print_last_error(_T("GetModuleHandle"));
        goto Cleanup;
    }

    /* Get LoadLibrary address */
    //_tprintf(_T("\t[+] Looking for LoadLibrary in kernel32\n"));
    LoadLibraryAddress = hash_GetProcAddress(hKernel32, (LPCSTR)PRINT_HIDE_STR("LoadLibraryW"));
    if (LoadLibraryAddress == NULL) {
        //print_last_error(_T("GetProcAddress"));
        goto Cleanup;
    }
    //_tprintf(_T("\t[+] Found at 0x%p\n"), LoadLibraryAddress);

    /* Get the full path of the dll */
    GetFullPathName(lpDllName, MAX_PATH, lpDllPath, NULL);
    //_tprintf(_T("\t[+] Full DLL Path: %s\n"), lpDllPath);

    /* Calculate the number of bytes needed for the DLL's pathname */
    dwSize = _tcslen(lpDllPath) * sizeof(TCHAR);

    /* Allocate memory into the remote process */
    //_tprintf(_T("\t[+] Allocating space for the path of the DLL\n"));
    lpBaseAddress = hash_VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpBaseAddress == NULL) {
       // print_last_error(_T("VirtualAllocEx"));
        goto Cleanup;
    }

    /* Write to the remote process */
    //printf("\t[+] Writing into the current process space at 0x%p\n", lpBaseAddress);
    if (!hash_WriteProcessMemory(hProcess, lpBaseAddress, lpDllPath, dwSize, NULL)) {
        //print_last_error(_T("WriteProcessMemory"));
        goto Cleanup;
    }

    /* Create the more thread */
    hRemoteThread = hash_CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryAddress, lpBaseAddress, NULL, 0);
    if (hRemoteThread == NULL) {
        //print_last_error(_T("CreateRemoteThread"));
    }
    else {
       // _tprintf(_T("Remote thread has been created successfully ...\n"));
        hash_WaitForSingleObject(hRemoteThread, INFINITE);
        hash_CloseHandle(hRemoteThread);

        /* assign function success return result */
        bStatus = TRUE;
    }

Cleanup:
    /* If lpBaseAddress initialized then hProcess is initialized too because of upper check. */
    if (lpBaseAddress) {
        hash_VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
    }
    if (hProcess)hash_CloseHandle(hProcess);

    return bStatus;
}


typedef NTSTATUS(WINAPI* pNtDelayExecution)(IN BOOLEAN, IN PLARGE_INTEGER);
BOOL timing_NtDelayexecution(UINT delayInMillis)
{
    // In this example, I will demonstrate NtDelayExecution because it is the lowest user mode
    // api to delay execution Sleep -> SleepEx -> NtDelayExecution.
    LARGE_INTEGER DelayInterval;
    LONGLONG llDelay = delayInMillis * 10000LL;
    DelayInterval.QuadPart = -llDelay;

    if (!IsAvailable(API_IDENTIFIER::API_NtDelayExecution))
        return TRUE; // TODO: make this a warning (NtDelayExecution should always exist)

    auto NtDelayExecution = static_cast<pNtDelayExecution>(GetAPI(API_IDENTIFIER::API_NtDelayExecution));
    NtDelayExecution(FALSE, &DelayInterval);

    return FALSE;
}

BOOL bProcessed = FALSE;

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime)
{
    // Malicious code is place here ....
    bProcessed = TRUE;
}


BOOL timing_SetTimer(UINT delayInMillis)
{
    MSG Msg;
    UINT_PTR iTimerID;

    // Set our timer without window handle
    iTimerID = SetTimer(NULL, 0, delayInMillis, TimerProc);

    if (iTimerID == NULL)
        return TRUE;

    // Because we are running in a console app, we should get the messages from
    // the queue and check if msg is WM_TIMER
    while (GetMessage(&Msg, NULL, 0, 0) & !bProcessed)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    // Kill the timer
    KillTimer(NULL, iTimerID);

    return FALSE;
}




BOOL timing_sleep_loop(UINT delayInMillis)
{
    /*
    This trick is about performing a low number of seconds to sleep but in a loop,
    the reason behind that sandboxes tries to avoid patching such sleeps because it
    could lead to race conditions and also because it is just negliable. However,
    when you do it in a loop, you can make it efficiant to cuz the sandboxe to reach
    its timeout.
    */

    int delayInMillis_divided = delayInMillis / 1000;

    /* Example: we want to sleep 300 seeconds, then we can sleep
    0.3s for 1000 times which is like: 300 seconds = 5 minues */
    for (int i = 0; i < 1000; i++) {
        hash_Sleep(delayInMillis_divided);
    }

    // Malicious code goes here

    return FALSE;
}


/*
RDSTC is a famous x86 instruction to count the number of cycle since reset.
This can be used to detect the VM. Thanks to Forcepoint for blog article.
*/

#define LODWORD(_qw)    ((DWORD)(_qw))
BOOL rdtsc_diff_locky()
{
    ULONGLONG tsc1;
    ULONGLONG tsc2;
    ULONGLONG tsc3;
    DWORD i = 0;

    // Try this 10 times in case of small fluctuations
    for (i = 0; i < 10; i++)
    {
        tsc1 = __rdtsc();

        // Waste some cycles - should be faster than CloseHandle on bare metal
        hash_GetProcessHeap();

        tsc2 = __rdtsc();

        // Waste some cycles - slightly longer than GetProcessHeap() on bare metal
        hash_CloseHandle(0);

        tsc3 = __rdtsc();

        // Did it take at least 10 times more CPU cycles to perform CloseHandle than it took to perform GetProcessHeap()?
        if ((LODWORD(tsc3) - LODWORD(tsc2)) / (LODWORD(tsc2) - LODWORD(tsc1)) >= 10)
            return FALSE;
    }

    // We consistently saw a small ratio of difference between GetProcessHeap and CloseHandle execution times
    // so we're probably in a VM!
    return TRUE;
}


/*
CPUID is an instruction which cauz a VM Exit to the VMM,
this little overhead can show the presence of a hypervisor
*/

BOOL rdtsc_diff_vmexit()
{
    ULONGLONG tsc1 = 0;
    ULONGLONG tsc2 = 0;
    ULONGLONG avg = 0;
    INT cpuInfo[4] = {};

    // Try this 10 times in case of small fluctuations
    for (INT i = 0; i < 10; i++)
    {
        tsc1 = __rdtsc();
        __cpuid(cpuInfo, 0);
        tsc2 = __rdtsc();

        // Get the delta of the two RDTSC
        avg += (tsc2 - tsc1);
    }

    // We repeated the process 10 times so we make sure our check is as much reliable as we can
    avg = avg / 10;
    return (avg < 1000 && avg > 0) ? FALSE : TRUE;
}


/*
Another timinig attack using the API IcmpSendEcho which takes a TimeOut
in milliseconds as a parameter, to wait for IPv4 ICMP packets replies.
First time observed: http://blog.talosintelligence.com/2017/09/avast-distributes-malware.html
*/

//BOOL timing_IcmpSendEcho(UINT delayInMillis)
//{
//
//    HANDLE hIcmpFile;
//    unsigned long DestinationAddress = INADDR_NONE;
//    char SendData[32] = "Data Buffer";
//    LPVOID ReplyBuffer = NULL;
//    DWORD ReplySize = 0;
//    const char ipaddr[] = "224.0.0.0";
//
//    hIcmpFile = IcmpCreateFile();
//    if (hIcmpFile == INVALID_HANDLE_VALUE) {
//        //printf("\tUnable to open handle.\n");
//        //printf("IcmpCreatefile returned error: %u\n", GetLastError());
//        return TRUE;
//    }
//
//
//    //
//    // Size of ICMP_ECHO_REPLY + size of send data + 8 extra bytes for ICMP error message
//    //
//    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8;
//    ReplyBuffer = (VOID*)malloc(ReplySize);
//    if (ReplyBuffer == NULL) {
//        IcmpCloseHandle(hIcmpFile);
//        //printf("\tUnable to allocate memory\n");
//        return TRUE;
//    }
//
//    IcmpSendEcho(hIcmpFile, DestinationAddress, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, delayInMillis);
//    IcmpCloseHandle(hIcmpFile);
//    free(ReplyBuffer);
//
//    return FALSE;
//}

/*
Timing attack using waitable timers. Test fails if any of the calls return an error state.
*/
BOOL timing_CreateWaitableTimer(UINT delayInMillis)
{
    HANDLE hTimer;
    LARGE_INTEGER dueTime;

    BOOL bResult = FALSE;

    dueTime.QuadPart = delayInMillis * -10000LL;

    hTimer = CreateWaitableTimer(NULL, TRUE, NULL);

    if (hTimer == NULL)
    {
        return TRUE;
    }

    if (SetWaitableTimer(hTimer, &dueTime, 0, NULL, NULL, FALSE) == FALSE)
    {
        bResult = TRUE;
    }
    else {
        if (hash_WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
        {
            bResult = TRUE;
        }
    }

    CancelWaitableTimer(hTimer);
    hash_CloseHandle(hTimer);
    return bResult;
}

HANDLE g_hEventCTQT = NULL;
VOID CALLBACK CallbackCTQT(PVOID lParam, BOOLEAN TimerOrWaitFired);
/*
Timing attack using CreateTimerQueueTimer. Test fails if any of the calls return an error state.
*/
BOOL timing_CreateTimerQueueTimer(UINT delayInMillis)
{
    HANDLE hTimerQueue;
    HANDLE hTimerQueueTimer = NULL;
    BOOL bResult = FALSE;

    g_hEventCTQT = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (g_hEventCTQT == NULL)
        return FALSE;

    hTimerQueue = CreateTimerQueue();
    if (hTimerQueue == NULL)
    {
        return TRUE;
    }

    if (CreateTimerQueueTimer(
        &hTimerQueueTimer,
        hTimerQueue,
        &CallbackCTQT,
        reinterpret_cast<PVOID>(0xDEADBEEFULL),
        delayInMillis,
        0,
        WT_EXECUTEDEFAULT) == FALSE)
    {
        bResult = TRUE;
    }
    else {

        // idea here is to wait only 10x the expected delay time
        // if the wait expires before the timer comes back, we fail the test
        if (hash_WaitForSingleObject(g_hEventCTQT, delayInMillis * 10) != WAIT_OBJECT_0)
        {
            bResult = FALSE;
        }

    }

    // Delete all timers in the timer queue.
    DeleteTimerQueueEx(hTimerQueue, NULL);

    hash_CloseHandle(g_hEventCTQT);

    return bResult;
}

VOID CALLBACK CallbackCTQT(PVOID lParam, BOOLEAN TimerOrWaitFired)
{
    if (TimerOrWaitFired == TRUE && lParam == reinterpret_cast<PVOID>(0xDEADBEEFULL))
    {
        SetEvent(g_hEventCTQT);
    }
}





BOOL bIsBeinDbg = TRUE;

LONG WINAPI UnhandledExcepFilter(PEXCEPTION_POINTERS pExcepPointers)
{
    // If a debugger is present, then this function will not be reached.
    bIsBeinDbg = FALSE;
    return EXCEPTION_CONTINUE_EXECUTION;
}


BOOL UnhandledExcepFilterTest()
{
    LPTOP_LEVEL_EXCEPTION_FILTER Top = hash_SetUnhandledExceptionFilter(UnhandledExcepFilter);
    RaiseException(EXCEPTION_FLT_DIVIDE_BY_ZERO, 0, 0, NULL);
    hash_SetUnhandledExceptionFilter(Top);
    return bIsBeinDbg;
}

static BOOL SwallowedException3 = TRUE;

static LONG CALLBACK VectoredHandler3(
    _In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
    SwallowedException3 = FALSE;
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
    {
        //Increase EIP/RIP to continue execution.
#ifdef _WIN64
        ExceptionInfo->ContextRecord->Rip++;
#else
        ExceptionInfo->ContextRecord->Eip++;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}


BOOL TrapFlag()
{
    PVOID Handle = AddVectoredExceptionHandler(1, VectoredHandler3);
    SwallowedException3 = TRUE;

#ifdef _WIN64
    UINT64 eflags = __readeflags();
#else
    UINT eflags = __readeflags();
#endif

    //  Set the trap flag
    eflags |= 0x100;
    __writeeflags(eflags);

    RemoveVectoredExceptionHandler(Handle);
    return SwallowedException3;
}
HANDLE hMutex;
BOOL checktry() {
    __try {
        /* Then, let's try close it */
        hash_CloseHandle(hMutex);
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return TRUE;
    }
}

BOOL SetHandleInformatiom_ProtectedHandle()
{


    /* Create a mutex so we can get a handle */
    hMutex = CreateMutex(NULL, FALSE, (LPCWSTR)PRINT_HIDE_STR("Goog"));

    if (hMutex) {

        /* Protect our handle */
        hash_SetHandleInformation(hMutex, HANDLE_FLAG_PROTECT_FROM_CLOSE, HANDLE_FLAG_PROTECT_FROM_CLOSE);


        checktry();

    }

    return FALSE;

}

static volatile HANDLE tls_callback_thread_event = 0;
static volatile HANDLE tls_callback_process_event = 0;
static volatile UINT32 tls_callback_thread_data = 0;
static volatile UINT32 tls_callback_process_data = 0;

volatile bool has_run = false;

VOID WINAPI tls_callback(PVOID hModule, DWORD dwReason, PVOID pContext)
{
    if (!has_run)
    {
        has_run = true;
        tls_callback_thread_event = CreateEvent(NULL, FALSE, FALSE, NULL);
        tls_callback_process_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    }

    if (dwReason == DLL_THREAD_ATTACH)
    {
       // OutputDebugString(L"TLS callback: thread attach");
        tls_callback_thread_data = 0xDEADBEEF;
        SetEvent(tls_callback_thread_event);
    }

    if (dwReason == DLL_PROCESS_ATTACH)
    {
       // OutputDebugString(L"TLS callback: process attach");
        tls_callback_process_data = 0xDEADBEEF;
        SetEvent(tls_callback_process_event);
    }
}

DWORD WINAPI TLSCallbackDummyThread(
    _In_ LPVOID lpParameter
)
{
    //OutputDebugString(L"TLS callback: dummy thread launched");
    return 0;
}

BOOL TLSCallbackThread()
{
    const int BLOWN = 1000;

    if (CreateThread(NULL, 0, &TLSCallbackDummyThread, NULL, 0, NULL) == NULL)
    {
        //OutputDebugString(L"TLS callback: couldn't start dummy thread");
    }

    int fuse = 0;
    while (tls_callback_thread_event == NULL && ++fuse != BLOWN) { SwitchToThread(); }
    if (fuse >= BLOWN)
    {
        //OutputDebugString(L"TLSCallbackThread timeout on event creation.");
        return TRUE;
    }

    DWORD waitStatus = WaitForSingleObject(tls_callback_thread_event, 5000);
    if (waitStatus != WAIT_OBJECT_0)
    {
        if (waitStatus == WAIT_FAILED) {}
            //OutputDebugString(L"TLSCallbackThread wait failed.");
        else if (waitStatus == WAIT_ABANDONED) {}
            //OutputDebugString(L"TLSCallbackThread wait abandoned.");
        else {}
           // OutputDebugString(L"TLSCallbackThread timeout on event wait.");
        return TRUE;
    }

    if (tls_callback_thread_data != 0xDEADBEEF){}
        //OutputDebugString(L"TLSCallbackThread data did not match.");
    else{}
        //OutputDebugString(L"All seems fine for TLSCallbackThread.");

    return tls_callback_thread_data == 0xDEADBEEF ? FALSE : TRUE;
}

BOOL TLSCallbackProcess()
{
    const int BLOWN = 1000;

    int fuse = 0;
    while (tls_callback_process_event == NULL && ++fuse != BLOWN) { SwitchToThread(); }
    if (fuse >= BLOWN)
    {
        //OutputDebugString(L"TLSCallbackProcess timeout on event creation.");
        return TRUE;
    }

    DWORD waitStatus = WaitForSingleObject(tls_callback_process_event, 5000);
    if (waitStatus != WAIT_OBJECT_0)
    {
        if (waitStatus == WAIT_FAILED){}
            //OutputDebugString(L"TLSCallbackProcess wait failed.");
        else if (waitStatus == WAIT_ABANDONED){}
            //OutputDebugString(L"TLSCallbackProcess wait abandoned.");
        else{}
            //OutputDebugString(L"TLSCallbackProcess timeout on event wait.");
        return TRUE;
    }

    if (tls_callback_process_data != 0xDEADBEEF){}
       // OutputDebugString(L"TLSCallbackProcess data did not match.");
    else{}
        //OutputDebugString(L"All seems fine for TLSCallbackProcess.");

    return tls_callback_process_data == 0xDEADBEEF ? FALSE : TRUE;
}

#ifdef _WIN64
#pragma comment (linker, "/INCLUDE:_tls_used")
#pragma comment (linker, "/INCLUDE:tls_callback_func")
#else
#pragma comment (linker, "/INCLUDE:__tls_used")
#pragma comment (linker, "/INCLUDE:_tls_callback_func")
#endif


#ifdef _WIN64
#pragma const_seg(".CRT$XLF")
EXTERN_C const
#else
#pragma data_seg(".CRT$XLF")
EXTERN_C
#endif

PIMAGE_TLS_CALLBACK tls_callback_func = tls_callback;

#ifdef _WIN64
#pragma const_seg()
#else
#pragma data_seg()
#endif //_WIN64


#ifdef _DEBUG
#define OutputDebugStringDbgOnly(S) OutputDebugString(S)
#else
#define OutputDebugStringDbgOnly(S) do {} while(0);
#endif


std::vector<PVOID> executablePages = {};



typedef NTSTATUS(WINAPI* pNtSetInformationThread)(HANDLE, UINT, PVOID, ULONG);
typedef NTSTATUS(WINAPI* pNtQueryInformationThread)(HANDLE, UINT, PVOID, ULONG, PULONG);
BOOL NtSetInformationThread_ThreadHideFromDebugger()
{
    // ThreadHideFromDebugger
    const int ThreadHideFromDebugger = 0x11;

    auto NtSetInformationThread = static_cast<pNtSetInformationThread>(GetAPI(API_IDENTIFIER::API_NtSetInformationThread));
    auto NtQueryInformationThread = static_cast<pNtQueryInformationThread>(GetAPI(API_IDENTIFIER::API_NtQueryInformationThread));

    NTSTATUS Status;
    bool doQITcheck = false;

    // only do the QueryInformationThread check if we're on Vista and the API is available.
    // this is because the ThreadHideFromDebugger class can only be queried from Vista onwards.
    if (IsAvailable(API_IDENTIFIER::API_NtQueryInformationThread))
    {
        doQITcheck = IsWindowsVistaOrGreater();
    }

    BOOL isThreadHidden = FALSE;

    // First issue a bogus call with an incorrect length parameter. If it succeeds, we know NtSetInformationThread was hooked.
    Status = NtSetInformationThread(hash_GetCurrentThread(), ThreadHideFromDebugger, &isThreadHidden, 12345);
    if (Status == 0)
        return TRUE;

    // Next try again but give it a bogus thread handle. If it succeeds, again we know NtSetInformationThread was hooked.
    Status = NtSetInformationThread((HANDLE)0xFFFF, ThreadHideFromDebugger, NULL, 0);
    if (Status == 0)
        return TRUE;

    // Now try a legitimate call.
    Status = NtSetInformationThread(hash_GetCurrentThread(), ThreadHideFromDebugger, NULL, 0);

    if (Status == 0)
    {
        if (doQITcheck)
        {
            Status = NtQueryInformationThread(hash_GetCurrentThread(), ThreadHideFromDebugger, &isThreadHidden, sizeof(BOOL), NULL);
            if (Status == 0)
            {
                // if the thread isn't hidden we know the ThreadHideFromDebugger call didn't do what it told us it did
                return isThreadHidden ? FALSE : TRUE;
            }
        }
    }
    else
    {
        // call failed, should've succeeded
        return TRUE;
    }

    // we didn't find any hooks.
    return FALSE;
}

char apis_kernel32[] = "ActivateActCtx ActivateActCtxWorker AddAtomA AddAtomW AddConsoleAliasA AddConsoleAliasW AddIntegrityLabelToBoundaryDescriptor AddLocalAlternateComputerNameA AddLocalAlternateComputerNameW AddRefActCtx AddRefActCtxWorker AddResourceAttributeAce AddSIDToBoundaryDescriptor AddScopedPolicyIDAce AddSecureMemoryCacheCallback AdjustCalendarDate AllocConsole AllocateUserPhysicalPages AllocateUserPhysicalPagesNuma ApplicationRecoveryFinished ApplicationRecoveryInProgress AreFileApisANSI AssignProcessToJobObject AttachConsole BackupRead BackupSeek BackupWrite BaseCheckAppcompatCache BaseCheckAppcompatCacheEx BaseCheckAppcompatCacheExWorker BaseCheckAppcompatCacheWorker BaseCheckElevation BaseCleanupAppcompatCacheSupport BaseCleanupAppcompatCacheSupportWorker BaseDestroyVDMEnvironment BaseDllReadWriteIniFile BaseDumpAppcompatCache BaseDumpAppcompatCacheWorker BaseElevationPostProcessing BaseFlushAppcompatCache BaseFlushAppcompatCacheWorker BaseFormatObjectAttributes BaseFormatTimeOut BaseFreeAppCompatDataForProcessWorker BaseGenerateAppCompatData BaseGetNamedObjectDirectory BaseInitAppcompatCacheSupport BaseInitAppcompatCacheSupportWorker BaseIsAppcompatInfrastructureDisabled BaseIsAppcompatInfrastructureDisabledWorker BaseIsDosApplication BaseQueryModuleData BaseReadAppCompatDataForProcessWorker BaseSetLastNTError BaseThreadInitThunk BaseUpdateAppcompatCache BaseUpdateAppcompatCacheWorker BaseUpdateVDMEntry BaseVerifyUnicodeString BaseWriteErrorElevationRequiredEvent Basep8BitStringToDynamicUnicodeString BasepAllocateActivationContextActivationBlock BasepAnsiStringToDynamicUnicodeString BasepAppContainerEnvironmentExtension BasepAppXExtension BasepCheckAppCompat BasepCheckWebBladeHashes BasepCheckWinSaferRestrictions BasepConstructSxsCreateProcessMessage BasepCopyEncryption BasepFreeActivationContextActivationBlock BasepFreeAppCompatData BasepGetAppCompatData BasepGetComputerNameFromNtPath BasepGetExeArchType BasepInitAppCompatData BasepIsProcessAllowed " \
"BasepMapModuleHandle BasepNotifyLoadStringResource BasepPostSuccessAppXExtension BasepProcessInvalidImage BasepQueryAppCompat BasepQueryModuleChpeSettings BasepReleaseAppXContext BasepReleaseSxsCreateProcessUtilityStruct BasepReportFault BasepSetFileEncryptionCompression Beep BeginUpdateResourceA BeginUpdateResourceW BindIoCompletionCallback BuildCommDCBA BuildCommDCBAndTimeoutsA BuildCommDCBAndTimeoutsW BuildCommDCBW CallNamedPipeA CallNamedPipeW CallbackMayRunLong CancelDeviceWakeupRequest CancelIo CancelIoEx CancelSynchronousIo CancelTimerQueueTimer CancelWaitableTimer ChangeTimerQueueTimer CheckAllowDecryptedRemoteDestinationPolicy CheckElevation CheckElevationEnabled CheckForReadOnlyResource CheckForReadOnlyResourceFilter CheckNameLegalDOS8Dot3A CheckNameLegalDOS8Dot3W CheckRemoteDebuggerPresent CheckTokenCapability CheckTokenMembershipEx ClearCommBreak ClearCommError CloseConsoleHandle CloseHandle ClosePrivateNamespace CloseProfileUserMapping CmdBatNotification CommConfigDialogA CommConfigDialogW CompareCalendarDates CompareFileTime CompareStringA CompareStringEx CompareStringOrdinal CompareStringW ConnectNamedPipe ConsoleMenuControl ContinueDebugEvent ConvertCalDateTimeToSystemTime ConvertDefaultLocale ConvertFiberToThread ConvertNLSDayOfWeekToWin32DayOfWeek ConvertSystemTimeToCalDateTime ConvertThreadToFiber ConvertThreadToFiberEx CopyContext CopyFile2 CopyFileA CopyFileExA CopyFileExW CopyFileTransactedA CopyFileTransactedW CopyFileW CopyLZFile CreateActCtxA CreateActCtxW CreateActCtxWWorker CreateBoundaryDescriptorA CreateBoundaryDescriptorW CreateConsoleScreenBuffer CreateDirectoryA CreateDirectoryExA CreateDirectoryExW CreateDirectoryTransactedA CreateDirectoryTransactedW CreateDirectoryW CreateEventA CreateEventExA CreateEventExW CreateEventW CreateFiber CreateFiberEx CreateFile2 CreateFileA CreateFileMappingA CreateFileMappingNumaA CreateFileMappingNumaW CreateFileMappingW CreateFileTransactedA CreateFileTransactedW CreateFileW CreateHardLinkA CreateHardLinkTransactedA " \
"CreateHardLinkTransactedW CreateHardLinkW CreateIoCompletionPort CreateJobObjectA CreateJobObjectW CreateJobSet CreateMailslotA CreateMailslotW CreateMemoryResourceNotification CreateMutexA CreateMutexExA CreateMutexExW CreateMutexW CreateNamedPipeA CreateNamedPipeW CreatePipe CreatePrivateNamespaceA CreatePrivateNamespaceW CreateProcessA CreateProcessAsUserA CreateProcessAsUserW CreateProcessInternalA CreateProcessInternalW CreateProcessW CreateRemoteThread CreateSemaphoreA CreateSemaphoreExA CreateSemaphoreExW CreateSemaphoreW CreateSymbolicLinkA CreateSymbolicLinkTransactedA CreateSymbolicLinkTransactedW CreateSymbolicLinkW CreateTapePartition CreateThread CreateThreadpool CreateThreadpoolCleanupGroup CreateThreadpoolIo CreateThreadpoolTimer CreateThreadpoolWait CreateThreadpoolWork CreateTimerQueue CreateTimerQueueTimer CreateToolhelp32Snapshot CreateUmsCompletionList CreateUmsThreadContext CreateWaitableTimerA CreateWaitableTimerExA CreateWaitableTimerExW CreateWaitableTimerW DeactivateActCtx DeactivateActCtxWorker DebugActiveProcess DebugActiveProcessStop DebugBreak DebugBreakProcess DebugSetProcessKillOnExit DefineDosDeviceA DefineDosDeviceW DelayLoadFailureHook DeleteAtom DeleteBoundaryDescriptor DeleteFiber DeleteFileA DeleteFileTransactedA DeleteFileTransactedW DeleteFileW DeleteSynchronizationBarrier DeleteTimerQueue DeleteTimerQueueEx DeleteTimerQueueTimer DeleteUmsCompletionList DeleteUmsThreadContext DeleteVolumeMountPointA DeleteVolumeMountPointW DequeueUmsCompletionListItems DeviceIoControl DisableThreadLibraryCalls DisableThreadProfiling DisconnectNamedPipe DnsHostnameToComputerNameA DnsHostnameToComputerNameExW DnsHostnameToComputerNameW DosDateTimeToFileTime DosPathToSessionPathA DosPathToSessionPathW DuplicateConsoleHandle DuplicateEncryptionInfoFileExt DuplicateHandle EnableThreadProfiling EndUpdateResourceA EndUpdateResourceW EnterSynchronizationBarrier EnterUmsSchedulingMode EnumCalendarInfoA EnumCalendarInfoExA EnumCalendarInfoExEx EnumCalendarInfoExW " \
"EnumCalendarInfoW EnumDateFormatsA EnumDateFormatsExA EnumDateFormatsExEx EnumDateFormatsExW EnumDateFormatsW EnumLanguageGroupLocalesA EnumLanguageGroupLocalesW EnumResourceLanguagesA EnumResourceLanguagesExA EnumResourceLanguagesExW EnumResourceLanguagesW EnumResourceNamesA EnumResourceNamesExA EnumResourceNamesExW EnumResourceNamesW EnumResourceTypesA EnumResourceTypesExA EnumResourceTypesExW EnumResourceTypesW EnumSystemCodePagesA EnumSystemCodePagesW EnumSystemFirmwareTables EnumSystemGeoID EnumSystemGeoNames EnumSystemLanguageGroupsA EnumSystemLanguageGroupsW EnumSystemLocalesA EnumSystemLocalesEx EnumSystemLocalesW EnumTimeFormatsA EnumTimeFormatsEx EnumTimeFormatsW EnumUILanguagesA EnumUILanguagesW EnumerateLocalComputerNamesA EnumerateLocalComputerNamesW EraseTape EscapeCommFunction ExecuteUmsThread ExitProcess ExitVDM ExpandEnvironmentStringsA ExpandEnvironmentStringsW ExpungeConsoleCommandHistoryA ExpungeConsoleCommandHistoryW FatalAppExitA FatalAppExitW FatalExit FileTimeToDosDateTime FileTimeToLocalFileTime FileTimeToSystemTime FillConsoleOutputAttribute FillConsoleOutputCharacterA FillConsoleOutputCharacterW FindActCtxSectionGuid FindActCtxSectionGuidWorker FindActCtxSectionStringA FindActCtxSectionStringW FindActCtxSectionStringWWorker FindAtomA FindAtomW FindClose FindCloseChangeNotification FindFirstChangeNotificationA FindFirstChangeNotificationW FindFirstFileA FindFirstFileExA FindFirstFileExW FindFirstFileNameTransactedW FindFirstFileNameW FindFirstFileTransactedA FindFirstFileTransactedW FindFirstFileW FindFirstStreamTransactedW FindFirstVolumeA FindFirstVolumeMountPointA FindFirstVolumeMountPointW FindFirstVolumeW FindNLSString FindNLSStringEx FindNextChangeNotification FindNextFileA FindNextFileNameW FindNextFileW FindNextVolumeA FindNextVolumeMountPointA FindNextVolumeMountPointW FindNextVolumeW FindResourceA FindResourceExA FindResourceExW FindResourceW FindStringOrdinal FindVolumeClose FindVolumeMountPointClose FlsAlloc FlsFree FlsGetValue FlsSetValue " \
"FlushConsoleInputBuffer FlushFileBuffers FlushInstructionCache FlushViewOfFile FoldStringA FoldStringW FormatMessageA FormatMessageW FreeConsole FreeEnvironmentStringsA FreeEnvironmentStringsW FreeLibrary FreeLibraryAndExitThread FreeMemoryJobObject FreeResource FreeUserPhysicalPages GenerateConsoleCtrlEvent GetACP GetActiveProcessorCount GetActiveProcessorGroupCount GetAppContainerAce GetAppContainerNamedObjectPath GetApplicationRecoveryCallback GetApplicationRecoveryCallbackWorker GetApplicationRestartSettings GetApplicationRestartSettingsWorker GetAtomNameA GetAtomNameW GetBinaryType GetBinaryTypeA GetBinaryTypeW GetCPInfo GetCPInfoExA GetCPInfoExW GetCachedSigningLevel GetCalendarDateFormat GetCalendarDateFormatEx GetCalendarDaysInMonth GetCalendarDifferenceInDays GetCalendarInfoA GetCalendarInfoEx GetCalendarInfoW GetCalendarMonthsInYear GetCalendarSupportedDateRange GetCalendarWeekNumber GetComPlusPackageInstallStatus GetCommConfig GetCommMask GetCommModemStatus GetCommProperties GetCommState GetCommTimeouts GetCommandLineA GetCommandLineW GetCompressedFileSizeA GetCompressedFileSizeTransactedA GetCompressedFileSizeTransactedW GetCompressedFileSizeW GetComputerNameA GetComputerNameExA GetComputerNameExW GetComputerNameW GetConsoleAliasA GetConsoleAliasExesA GetConsoleAliasExesLengthA GetConsoleAliasExesLengthW GetConsoleAliasExesW GetConsoleAliasW GetConsoleAliasesA GetConsoleAliasesLengthA GetConsoleAliasesLengthW GetConsoleAliasesW GetConsoleCP GetConsoleCharType GetConsoleCommandHistoryA GetConsoleCommandHistoryLengthA GetConsoleCommandHistoryLengthW GetConsoleCommandHistoryW GetConsoleCursorInfo GetConsoleCursorMode GetConsoleDisplayMode GetConsoleFontInfo GetConsoleFontSize GetConsoleHardwareState GetConsoleHistoryInfo GetConsoleInputWaitHandle GetConsoleKeyboardLayoutNameA GetConsoleKeyboardLayoutNameW GetConsoleMode GetConsoleNlsMode GetConsoleOriginalTitleA GetConsoleOriginalTitleW GetConsoleOutputCP GetConsoleProcessList GetConsoleScreenBufferInfo GetConsoleScreenBufferInfoEx " \
"GetConsoleSelectionInfo GetConsoleTitleA GetConsoleTitleW GetConsoleWindow GetCurrencyFormatA GetCurrencyFormatEx GetCurrencyFormatW GetCurrentActCtx GetCurrentActCtxWorker GetCurrentConsoleFont GetCurrentConsoleFontEx GetCurrentDirectoryA GetCurrentDirectoryW GetCurrentProcess GetCurrentProcessId GetCurrentThread GetCurrentThreadId GetCurrentUmsThread GetDateFormatA GetDateFormatAWorker GetDateFormatEx GetDateFormatW GetDateFormatWWorker GetDefaultCommConfigA GetDefaultCommConfigW GetDevicePowerState GetDiskFreeSpaceA GetDiskFreeSpaceExA GetDiskFreeSpaceExW GetDiskFreeSpaceW GetDllDirectoryA GetDllDirectoryW GetDriveTypeA GetDriveTypeW GetDurationFormat GetDurationFormatEx GetDynamicTimeZoneInformation GetEnabledXStateFeatures GetEncryptedFileVersionExt GetEnvironmentStrings GetEnvironmentStringsA GetEnvironmentStringsW GetEnvironmentVariableA GetEnvironmentVariableW GetEraNameCountedString GetErrorMode GetExitCodeProcess GetExitCodeThread GetExpandedNameA GetExpandedNameW GetFileAttributesA GetFileAttributesExA GetFileAttributesExW GetFileAttributesTransactedA GetFileAttributesTransactedW GetFileAttributesW GetFileBandwidthReservation GetFileInformationByHandle GetFileInformationByHandleEx GetFileMUIInfo GetFileMUIPath GetFileSize GetFileSizeEx GetFileTime GetFileType GetFinalPathNameByHandleA GetFinalPathNameByHandleW GetFirmwareEnvironmentVariableA GetFirmwareEnvironmentVariableExA GetFirmwareEnvironmentVariableExW GetFirmwareEnvironmentVariableW GetFirmwareType GetFullPathNameA GetFullPathNameTransactedA GetFullPathNameTransactedW GetFullPathNameW GetGeoInfoA GetGeoInfoEx GetGeoInfoW GetHandleInformation GetLargePageMinimum GetLargestConsoleWindowSize GetLastError GetLocalTime GetLocaleInfoA GetLocaleInfoEx GetLocaleInfoW GetLogicalDriveStringsA GetLogicalDriveStringsW GetLogicalDrives GetLogicalProcessorInformation GetLongPathNameA GetLongPathNameTransactedA GetLongPathNameTransactedW GetLongPathNameW GetMailslotInfo GetMaximumProcessorCount GetMaximumProcessorGroupCount " \
"GetMemoryErrorHandlingCapabilities GetModuleFileNameA GetModuleFileNameW GetModuleHandleA GetModuleHandleExA GetModuleHandleExW GetModuleHandleW GetNLSVersion GetNLSVersionEx GetNamedPipeAttribute GetNamedPipeClientComputerNameA GetNamedPipeClientComputerNameW GetNamedPipeClientProcessId GetNamedPipeClientSessionId GetNamedPipeHandleStateA GetNamedPipeHandleStateW GetNamedPipeServerProcessId GetNamedPipeServerSessionId GetNativeSystemInfo GetNextUmsListItem GetNextVDMCommand GetNumaAvailableMemoryNode GetNumaAvailableMemoryNodeEx GetNumaHighestNodeNumber GetNumaNodeNumberFromHandle GetNumaNodeProcessorMask GetNumaNodeProcessorMaskEx GetNumaProcessorNode GetNumaProcessorNodeEx GetNumaProximityNode GetNumaProximityNodeEx GetNumberFormatA GetNumberFormatEx GetNumberFormatW GetNumberOfConsoleFonts GetNumberOfConsoleInputEvents GetNumberOfConsoleMouseButtons GetOEMCP GetOverlappedResult GetPhysicallyInstalledSystemMemory GetPriorityClass GetPrivateProfileIntA GetPrivateProfileIntW GetPrivateProfileSectionA GetPrivateProfileSectionNamesA GetPrivateProfileSectionNamesW GetPrivateProfileSectionW GetPrivateProfileStringA GetPrivateProfileStringW GetPrivateProfileStructA GetPrivateProfileStructW GetProcAddress GetProcessAffinityMask GetProcessDEPPolicy GetProcessGroupAffinity GetProcessHandleCount GetProcessHeap GetProcessHeaps GetProcessId GetProcessIdOfThread GetProcessInformation GetProcessIoCounters GetProcessPreferredUILanguages GetProcessPriorityBoost GetProcessShutdownParameters GetProcessTimes GetProcessVersion GetProcessWorkingSetSize GetProcessWorkingSetSizeEx GetProductInfo GetProfileIntA GetProfileIntW GetProfileSectionA GetProfileSectionW GetProfileStringA GetProfileStringW GetQueuedCompletionStatus GetQueuedCompletionStatusEx GetShortPathNameA GetShortPathNameW GetStartupInfoA GetStartupInfoW GetStdHandle GetStringScripts GetStringTypeA GetStringTypeExA GetStringTypeExW GetStringTypeW GetSystemDEPPolicy GetSystemDefaultLCID GetSystemDefaultLangID GetSystemDefaultLocaleName " \
"GetSystemDefaultUILanguage GetSystemDirectoryA GetSystemDirectoryW GetSystemFileCacheSize GetSystemFirmwareTable GetSystemInfo GetSystemPowerStatus GetSystemPreferredUILanguages GetSystemRegistryQuota GetSystemTime GetSystemTimeAdjustment GetSystemTimeAsFileTime GetSystemTimePreciseAsFileTime GetSystemTimes GetSystemWindowsDirectoryA GetSystemWindowsDirectoryW GetSystemWow64DirectoryA GetSystemWow64DirectoryW GetTapeParameters GetTapePosition GetTapeStatus GetTempFileNameA GetTempFileNameW GetTempPathA GetTempPathW GetThreadContext GetThreadErrorMode GetThreadGroupAffinity GetThreadIOPendingFlag GetThreadId GetThreadIdealProcessorEx GetThreadInformation GetThreadLocale GetThreadPreferredUILanguages GetThreadPriority GetThreadPriorityBoost GetThreadSelectorEntry GetThreadTimes GetThreadUILanguage GetTickCount GetTickCount64 GetTimeFormatA GetTimeFormatAWorker GetTimeFormatEx GetTimeFormatW GetTimeFormatWWorker GetTimeZoneInformation GetTimeZoneInformationForYear GetUILanguageInfo GetUmsCompletionListEvent GetUmsSystemThreadInformation GetUserDefaultGeoName GetUserDefaultLCID GetUserDefaultLangID GetUserDefaultLocaleName GetUserDefaultUILanguage GetUserGeoID GetUserPreferredUILanguages GetVDMCurrentDirectories GetVersion GetVersionExA GetVersionExW GetVolumeInformationA GetVolumeInformationByHandleW GetVolumeInformationW GetVolumeNameForVolumeMountPointA GetVolumeNameForVolumeMountPointW GetVolumePathNameA GetVolumePathNameW GetVolumePathNamesForVolumeNameA GetVolumePathNamesForVolumeNameW GetWindowsDirectoryA GetWindowsDirectoryW GetWriteWatch GetXStateFeaturesMask GlobalAddAtomA GlobalAddAtomExA GlobalAddAtomExW GlobalAddAtomW GlobalAlloc GlobalCompact GlobalDeleteAtom GlobalFindAtomA GlobalFindAtomW GlobalFix GlobalFlags GlobalFree GlobalGetAtomNameA GlobalGetAtomNameW GlobalHandle GlobalLock GlobalMemoryStatus GlobalMemoryStatusEx GlobalReAlloc GlobalSize GlobalUnWire GlobalUnfix GlobalUnlock GlobalWire Heap32First Heap32ListFirst Heap32ListNext Heap32Next HeapCompact " \
"HeapCreate HeapDestroy HeapFree HeapLock HeapQueryInformation HeapSetInformation HeapSummary HeapUnlock HeapValidate HeapWalk IdnToAscii IdnToNameprepUnicode IdnToUnicode InitAtomTable InitializeContext InitializeCriticalSectionAndSpinCount InitializeCriticalSectionEx InitializeSynchronizationBarrier InvalidateConsoleDIBits IsBadCodePtr IsBadHugeReadPtr IsBadHugeWritePtr IsBadReadPtr IsBadStringPtrA IsBadStringPtrW IsBadWritePtr IsCalendarLeapDay IsCalendarLeapMonth IsCalendarLeapYear IsDBCSLeadByte IsDBCSLeadByteEx IsDebuggerPresent IsNLSDefinedString IsNativeVhdBoot IsNormalizedString IsProcessInJob IsProcessorFeaturePresent IsSystemResumeAutomatic IsThreadAFiber IsValidCalDateTime IsValidCodePage IsValidLanguageGroup IsValidLocale IsValidLocaleName IsValidNLSVersion IsWow64Process K32EmptyWorkingSet K32EnumDeviceDrivers K32EnumPageFilesA K32EnumPageFilesW K32EnumProcessModules K32EnumProcessModulesEx K32EnumProcesses K32GetDeviceDriverBaseNameA K32GetDeviceDriverBaseNameW K32GetDeviceDriverFileNameA K32GetDeviceDriverFileNameW K32GetMappedFileNameA K32GetMappedFileNameW K32GetModuleBaseNameA K32GetModuleBaseNameW K32GetModuleFileNameExA K32GetModuleFileNameExW K32GetModuleInformation K32GetPerformanceInfo K32GetProcessImageFileNameA K32GetProcessImageFileNameW K32GetProcessMemoryInfo K32GetWsChanges K32GetWsChangesEx K32InitializeProcessForWsWatch K32QueryWorkingSet K32QueryWorkingSetEx LCIDToLocaleName LCMapStringA LCMapStringEx LCMapStringW LZClose LZCloseFile LZCopy LZCreateFileW LZDone LZInit LZOpenFileA LZOpenFileW LZRead LZSeek LZStart LoadAppInitDlls LoadLibraryA LoadLibraryExA LoadLibraryExW LoadLibraryW LoadModule LoadPackagedLibrary LoadResource LoadStringBaseExW LoadStringBaseW LocalAlloc LocalCompact LocalFileTimeToFileTime LocalFlags LocalFree LocalHandle LocalLock LocalReAlloc LocalShrink LocalSize LocalUnlock LocaleNameToLCID LocateXStateFeature LockFile LockFileEx LockResource MapUserPhysicalPages MapUserPhysicalPagesScatter MapViewOfFile MapViewOfFileEx " \
"MapViewOfFileExNuma Module32First Module32FirstW Module32Next Module32NextW MoveFileA MoveFileExA MoveFileExW MoveFileTransactedA MoveFileTransactedW MoveFileW MoveFileWithProgressA MoveFileWithProgressW MulDiv MultiByteToWideChar NeedCurrentDirectoryForExePathA NeedCurrentDirectoryForExePathW NlsCheckPolicy NlsEventDataDescCreate NlsGetCacheUpdateCount NlsUpdateLocale NlsUpdateSystemLocale NlsWriteEtwEvent NormalizeString NotifyMountMgr NotifyUILanguageChange NtVdm64CreateProcessInternalW OOBEComplete OpenConsoleW OpenConsoleWStub OpenEventA OpenEventW OpenFile OpenFileById OpenFileMappingA OpenFileMappingW OpenJobObjectA OpenJobObjectW OpenMutexA OpenMutexW OpenPrivateNamespaceA OpenPrivateNamespaceW OpenProcess OpenProfileUserMapping OpenSemaphoreA OpenSemaphoreW OpenThread OpenWaitableTimerA OpenWaitableTimerW OutputDebugStringA OutputDebugStringW PeekConsoleInputA PeekConsoleInputW PeekNamedPipe PostQueuedCompletionStatus PowerClearRequest PowerCreateRequest PowerSetRequest PrepareTape PrivCopyFileExW PrivMoveFileIdentityW Process32First Process32FirstW Process32Next Process32NextW ProcessIdToSessionId PssCaptureSnapshot PssDuplicateSnapshot PssFreeSnapshot PssQuerySnapshot PssWalkMarkerCreate PssWalkMarkerFree PssWalkMarkerGetPosition PssWalkMarkerRewind PssWalkMarkerSeek PssWalkMarkerSeekToBeginning PssWalkMarkerSetPosition PssWalkMarkerTell PssWalkSnapshot PulseEvent PurgeComm QueryActCtxSettingsW QueryActCtxSettingsWWorker QueryActCtxW QueryActCtxWWorker QueryDosDeviceA QueryDosDeviceW QueryFullProcessImageNameA QueryFullProcessImageNameW QueryIdleProcessorCycleTime QueryIdleProcessorCycleTimeEx QueryInformationJobObject QueryIoRateControlInformationJobObject QueryMemoryResourceNotification QueryPerformanceCounter QueryPerformanceFrequency QueryProcessAffinityUpdateMode QueryProcessCycleTime QueryThreadCycleTime QueryThreadProfiling QueryThreadpoolStackInformation QueryUmsThreadInformation QueryUnbiasedInterruptTime QueueUserAPC QueueUserWorkItem QuirkGetData2Worker " \
"QuirkGetDataWorker QuirkIsEnabled2Worker QuirkIsEnabled3Worker QuirkIsEnabledForPackage2Worker QuirkIsEnabledForPackage3Worker QuirkIsEnabledForPackage4Worker QuirkIsEnabledForPackageWorker QuirkIsEnabledForProcessWorker QuirkIsEnabledWorker RaiseException RaiseInvalid16BitExeError ReOpenFile ReadConsoleA ReadConsoleInputA ReadConsoleInputW ReadConsoleOutputA ReadConsoleOutputAttribute ReadConsoleOutputCharacterA ReadConsoleOutputCharacterW ReadConsoleOutputW ReadConsoleW ReadDirectoryChangesExW ReadDirectoryChangesW ReadFile ReadFileEx ReadFileScatter ReadProcessMemory ReadThreadProfilingData RegCloseKey RegCopyTreeW RegCreateKeyExA RegCreateKeyExW RegDeleteKeyExA RegDeleteKeyExW RegDeleteTreeA RegDeleteTreeW RegDeleteValueA RegDeleteValueW RegDisablePredefinedCacheEx RegEnumKeyExA RegEnumKeyExW RegEnumValueA RegEnumValueW RegFlushKey RegGetKeySecurity RegGetValueA RegGetValueW RegLoadKeyA RegLoadKeyW RegLoadMUIStringA RegLoadMUIStringW RegNotifyChangeKeyValue RegOpenCurrentUser RegOpenKeyExA RegOpenKeyExW RegOpenUserClassesRoot RegQueryInfoKeyA RegQueryInfoKeyW RegQueryValueExA RegQueryValueExW RegRestoreKeyA RegRestoreKeyW RegSaveKeyExA RegSaveKeyExW RegSetKeySecurity RegSetValueExA RegSetValueExW RegUnLoadKeyA RegUnLoadKeyW RegisterApplicationRecoveryCallback RegisterApplicationRestart RegisterBadMemoryNotification RegisterConsoleIME RegisterConsoleOS2 RegisterConsoleVDM RegisterWaitForInputIdle RegisterWaitForSingleObject RegisterWaitForSingleObjectEx RegisterWaitUntilOOBECompleted RegisterWowBaseHandlers RegisterWowExec ReleaseActCtx ReleaseActCtxWorker ReleaseMutex ReleaseSemaphore RemoveDirectoryA RemoveDirectoryTransactedA RemoveDirectoryTransactedW RemoveDirectoryW RemoveLocalAlternateComputerNameA RemoveLocalAlternateComputerNameW RemoveSecureMemoryCacheCallback ReplaceFile ReplaceFileA ReplaceFileW ReplacePartitionUnit RequestDeviceWakeup RequestWakeupLatency ResetEvent ResetWriteWatch ResolveLocaleName ResumeThread RtlAddFunctionTable RtlCaptureContext RtlCaptureStackBackTrace " \
"RtlCompareMemory RtlCopyMemory RtlDeleteFunctionTable RtlFillMemory RtlInstallFunctionTableCallback RtlLookupFunctionEntry RtlMoveMemory RtlPcToFileHeader RtlRaiseException RtlRestoreContext RtlUnwind RtlUnwindEx RtlVirtualUnwind ScrollConsoleScreenBufferA ScrollConsoleScreenBufferW SearchPathA SearchPathW SetCachedSigningLevel SetCalendarInfoA SetCalendarInfoW SetComPlusPackageInstallStatus SetCommBreak SetCommConfig SetCommMask SetCommState SetCommTimeouts SetComputerNameA SetComputerNameEx2W SetComputerNameExA SetComputerNameExW SetComputerNameW SetConsoleActiveScreenBuffer SetConsoleCP SetConsoleCtrlHandler SetConsoleCursor SetConsoleCursorInfo SetConsoleCursorMode SetConsoleCursorPosition SetConsoleDisplayMode SetConsoleFont SetConsoleHardwareState SetConsoleHistoryInfo SetConsoleIcon SetConsoleKeyShortcuts SetConsoleLocalEUDC SetConsoleMaximumWindowSize SetConsoleMenuClose SetConsoleMode SetConsoleNlsMode SetConsoleNumberOfCommandsA SetConsoleNumberOfCommandsW SetConsoleOS2OemFormat SetConsoleOutputCP SetConsolePalette SetConsoleScreenBufferInfoEx SetConsoleScreenBufferSize SetConsoleTextAttribute SetConsoleTitleA SetConsoleTitleW SetConsoleWindowInfo SetCurrentConsoleFontEx SetCurrentDirectoryA SetCurrentDirectoryW SetDefaultCommConfigA SetDefaultCommConfigW SetDllDirectoryA SetDllDirectoryW SetDynamicTimeZoneInformation SetEndOfFile SetEnvironmentStringsA SetEnvironmentStringsW SetEnvironmentVariableA SetEnvironmentVariableW SetErrorMode SetEvent SetFileApisToANSI SetFileApisToOEM SetFileAttributesA SetFileAttributesTransactedA SetFileAttributesTransactedW SetFileAttributesW SetFileBandwidthReservation SetFileCompletionNotificationModes SetFileInformationByHandle SetFileIoOverlappedRange SetFilePointer SetFilePointerEx SetFileShortNameA SetFileShortNameW SetFileTime SetFileValidData SetFirmwareEnvironmentVariableA SetFirmwareEnvironmentVariableExA SetFirmwareEnvironmentVariableExW SetFirmwareEnvironmentVariableW SetHandleCount SetHandleInformation SetInformationJobObject " \
"SetIoRateControlInformationJobObject SetLastError SetLocalPrimaryComputerNameA SetLocalPrimaryComputerNameW SetLocalTime SetLocaleInfoA SetLocaleInfoW SetMailslotInfo SetMessageWaitingIndicator SetNamedPipeAttribute SetNamedPipeHandleState SetPriorityClass SetProcessAffinityMask SetProcessAffinityUpdateMode SetProcessDEPPolicy SetProcessInformation SetProcessPreferredUILanguages SetProcessPriorityBoost SetProcessShutdownParameters SetProcessWorkingSetSize SetProcessWorkingSetSizeEx SetSearchPathMode SetStdHandle SetStdHandleEx SetSystemFileCacheSize SetSystemPowerState SetSystemTime SetSystemTimeAdjustment SetTapeParameters SetTapePosition SetTermsrvAppInstallMode SetThreadAffinityMask SetThreadContext SetThreadErrorMode SetThreadExecutionState SetThreadGroupAffinity SetThreadIdealProcessor SetThreadIdealProcessorEx SetThreadInformation SetThreadLocale SetThreadPreferredUILanguages SetThreadPriority SetThreadPriorityBoost SetThreadStackGuarantee SetThreadUILanguage SetThreadpoolStackInformation SetThreadpoolThreadMinimum SetTimeZoneInformation SetTimerQueueTimer SetUmsThreadInformation SetUnhandledExceptionFilter SetUserGeoID SetUserGeoName SetVDMCurrentDirectories SetVolumeLabelA SetVolumeLabelW SetVolumeMountPointA SetVolumeMountPointW SetVolumeMountPointWStub SetWaitableTimer SetXStateFeaturesMask SetupComm ShowConsoleCursor SignalObjectAndWait SizeofResource Sleep SleepEx SortCloseHandle SortGetHandle SuspendThread SwitchToFiber SwitchToThread SystemTimeToFileTime SystemTimeToTzSpecificLocalTime TerminateJobObject TerminateProcess TerminateThread TermsrvAppInstallMode TermsrvConvertSysRootToUserDir TermsrvCreateRegEntry TermsrvDeleteKey TermsrvDeleteValue TermsrvGetPreSetValue TermsrvGetWindowsDirectoryA TermsrvGetWindowsDirectoryW TermsrvOpenRegEntry TermsrvOpenUserClasses TermsrvRestoreKey TermsrvSetKeySecurity TermsrvSetValueKey TermsrvSyncUserIniFileExt Thread32First Thread32Next TlsAlloc TlsFree TlsGetValue TlsSetValue Toolhelp32ReadProcessMemory TransactNamedPipe " \
"TransmitCommChar TrySubmitThreadpoolCallback TzSpecificLocalTimeToSystemTime UTRegister UTUnRegister UmsThreadYield UnhandledExceptionFilter UnlockFile UnlockFileEx UnmapViewOfFile UnregisterApplicationRecoveryCallback UnregisterApplicationRestart UnregisterBadMemoryNotification UnregisterConsoleIME UnregisterWait UnregisterWaitEx UnregisterWaitUntilOOBECompleted UpdateCalendarDayOfWeek UpdateResourceA UpdateResourceW VDMConsoleOperation VDMOperationStarted VerLanguageNameA VerLanguageNameW VerifyConsoleIoHandle VerifyScripts VerifyVersionInfoA VerifyVersionInfoW VirtualAlloc VirtualAllocEx VirtualAllocExNuma VirtualFree VirtualFreeEx VirtualLock VirtualProtect VirtualProtectEx VirtualQuery VirtualQueryEx VirtualUnlock WTSGetActiveConsoleSessionId WaitCommEvent WaitForDebugEvent WaitForMultipleObjects WaitForMultipleObjectsEx WaitForSingleObject WaitForSingleObjectEx WaitNamedPipeA WaitNamedPipeW WerGetFlags WerGetFlagsWorker WerRegisterAdditionalProcess WerRegisterAppLocalDump WerRegisterCustomMetadata WerRegisterExcludedMemoryBlock WerRegisterFile WerRegisterFileWorker WerRegisterMemoryBlock WerRegisterMemoryBlockWorker WerRegisterRuntimeExceptionModule WerRegisterRuntimeExceptionModuleWorker WerSetFlags WerSetFlagsWorker WerUnregisterAdditionalProcess WerUnregisterAppLocalDump WerUnregisterCustomMetadata WerUnregisterExcludedMemoryBlock WerUnregisterFile WerUnregisterFileWorker WerUnregisterMemoryBlock WerUnregisterMemoryBlockWorker WerUnregisterRuntimeExceptionModule WerUnregisterRuntimeExceptionModuleWorker WerpGetDebugger WerpInitiateRemoteRecovery WerpLaunchAeDebug WerpNotifyLoadStringResourceWorker WerpNotifyUseStringResourceWorker WideCharToMultiByte WinExec Wow64DisableWow64FsRedirection Wow64EnableWow64FsRedirection Wow64GetThreadContext Wow64GetThreadSelectorEntry Wow64RevertWow64FsRedirection Wow64SetThreadContext Wow64SuspendThread WriteConsoleA WriteConsoleInputA WriteConsoleInputVDMA WriteConsoleInputVDMW WriteConsoleInputW WriteConsoleOutputA WriteConsoleOutputAttribute " \
"WriteConsoleOutputCharacterA WriteConsoleOutputCharacterW WriteConsoleOutputW WriteConsoleW WriteFile WriteFileEx WriteFileGather WritePrivateProfileSectionA WritePrivateProfileSectionW WritePrivateProfileStringA WritePrivateProfileStringW WritePrivateProfileStructA WritePrivateProfileStructW WriteProcessMemory WriteProfileSectionA WriteProfileSectionW WriteProfileStringA WriteProfileStringW WriteTapemark ZombifyActCtx ZombifyActCtxWorker _hread _hwrite _lclose _lcreat _llseek _lopen _lread _lwrite lstrcat lstrcatA lstrcatW lstrcmp lstrcmpA lstrcmpW lstrcmpi lstrcmpiA lstrcmpiW lstrcpy lstrcpyA lstrcpyW lstrcpyn lstrcpynA lstrcpynW lstrlen lstrlenA lstrlenW timeBeginPeriod timeEndPeriod timeGetDevCaps timeGetSystemTime timeGetTime uaw_lstrcmpW uaw_lstrcmpiW uaw_lstrlenW uaw_wcschr uaw_wcscpy uaw_wcsicmp uaw_wcslen uaw_wcsrchr";

char apis_ntdll[] = "A_SHAFinal A_SHAInit A_SHAUpdate AlpcAdjustCompletionListConcurrencyCount AlpcFreeCompletionListMessage AlpcGetCompletionListLastMessageInformation AlpcGetCompletionListMessageAttributes AlpcGetHeaderSize AlpcGetMessageAttribute AlpcGetMessageFromCompletionList AlpcGetOutstandingCompletionListMessageCount AlpcInitializeMessageAttribute AlpcMaxAllowedMessageLength AlpcRegisterCompletionList AlpcRegisterCompletionListWorkerThread AlpcRundownCompletionList AlpcUnregisterCompletionList AlpcUnregisterCompletionListWorkerThread ApiSetQueryApiSetPresence CsrAllocateCaptureBuffer CsrAllocateMessagePointer CsrCaptureMessageBuffer CsrCaptureMessageMultiUnicodeStringsInPlace CsrCaptureMessageString CsrCaptureTimeout CsrClientCallServer CsrClientConnectToServer CsrFreeCaptureBuffer CsrGetProcessId CsrIdentifyAlertableThread CsrSetPriorityClass CsrVerifyRegion DbgBreakPoint DbgPrint DbgPrintEx DbgPrintReturnControlC DbgPrompt DbgQueryDebugFilterState DbgSetDebugFilterState DbgUiConnectToDbg DbgUiContinue DbgUiConvertStateChangeStructure DbgUiConvertStateChangeStructureEx DbgUiDebugActiveProcess DbgUiGetThreadDebugObject DbgUiIssueRemoteBreakin DbgUiRemoteBreakin DbgUiSetThreadDebugObject DbgUiStopDebugging DbgUiWaitStateChange DbgUserBreakPoint EtwCheckCoverage EtwCreateTraceInstanceId EtwDeliverDataBlock EtwEnumerateProcessRegGuids EtwEventActivityIdControl EtwEventEnabled EtwEventProviderEnabled EtwEventRegister EtwEventSetInformation EtwEventUnregister EtwEventWrite EtwEventWriteEndScenario EtwEventWriteEx EtwEventWriteFull EtwEventWriteNoRegistration EtwEventWriteStartScenario EtwEventWriteString EtwEventWriteTransfer EtwGetTraceEnableFlags EtwGetTraceEnableLevel EtwGetTraceLoggerHandle EtwLogTraceEvent EtwNotificationRegister EtwNotificationUnregister EtwProcessPrivateLoggerRequest EtwRegisterSecurityProvider EtwRegisterTraceGuidsA EtwRegisterTraceGuidsW EtwReplyNotification EtwSendNotification EtwSetMark EtwTraceEventInstance EtwTraceMessage EtwTraceMessageVa " \
"EtwUnregisterTraceGuids EtwWriteUMSecurityEvent EtwpCreateEtwThread EtwpGetCpuSpeed EvtIntReportAuthzEventAndSourceAsync EvtIntReportEventAndSourceAsync ExpInterlockedPopEntrySListEnd ExpInterlockedPopEntrySListFault ExpInterlockedPopEntrySListResume KiRaiseUserExceptionDispatcher KiUserApcDispatcher KiUserCallbackDispatcher KiUserExceptionDispatcher KiUserInvertedFunctionTable LdrAccessResource LdrAddDllDirectory LdrAddLoadAsDataTable LdrAddRefDll LdrAppxHandleIntegrityFailure LdrCallEnclave LdrControlFlowGuardEnforced LdrCreateEnclave LdrDeleteEnclave LdrDisableThreadCalloutsForDll LdrEnumResources LdrEnumerateLoadedModules LdrFastFailInLoaderCallout LdrFindEntryForAddress LdrFindResourceDirectory_U LdrFindResourceEx_U LdrFindResource_U LdrFlushAlternateResourceModules LdrGetDllDirectory LdrGetDllFullName LdrGetDllHandle LdrGetDllHandleByMapping LdrGetDllHandleByName LdrGetDllHandleEx LdrGetDllPath LdrGetFailureData LdrGetFileNameFromLoadAsDataTable LdrGetKnownDllSectionHandle LdrGetProcedureAddress LdrGetProcedureAddressEx LdrGetProcedureAddressForCaller LdrInitShimEngineDynamic LdrInitializeEnclave LdrInitializeThunk LdrLoadAlternateResourceModule LdrLoadAlternateResourceModuleEx LdrLoadDll LdrLoadEnclaveModule LdrLockLoaderLock LdrOpenImageFileOptionsKey LdrProcessInitializationComplete LdrProcessRelocationBlock LdrProcessRelocationBlockEx LdrQueryImageFileExecutionOptions LdrQueryImageFileExecutionOptionsEx LdrQueryImageFileKeyOption LdrQueryModuleServiceTags LdrQueryOptionalDelayLoadedAPI LdrQueryProcessModuleInformation LdrRegisterDllNotification LdrRemoveDllDirectory LdrRemoveLoadAsDataTable LdrResFindResource LdrResFindResourceDirectory LdrResGetRCConfig LdrResRelease LdrResSearchResource LdrResolveDelayLoadedAPI LdrResolveDelayLoadsFromDll LdrRscIsTypeExist LdrSetAppCompatDllRedirectionCallback LdrSetDefaultDllDirectories LdrSetDllDirectory LdrSetDllManifestProber LdrSetImplicitPathOptions LdrSetMUICacheType LdrShutdownProcess LdrShutdownThread LdrStandardizeSystemPath " \
"LdrSystemDllInitBlock LdrUnloadAlternateResourceModule LdrUnloadAlternateResourceModuleEx LdrUnloadDll LdrUnlockLoaderLock LdrUnregisterDllNotification LdrUpdatePackageSearchPath LdrVerifyImageMatchesChecksum LdrVerifyImageMatchesChecksumEx LdrpResGetMappingSize LdrpResGetResourceDirectory MD4Final MD4Init MD4Update MD5Final MD5Init MD5Update NlsAnsiCodePage NlsMbCodePageTag NlsMbOemCodePageTag NtAcceptConnectPort NtAccessCheck NtAccessCheckAndAuditAlarm NtAccessCheckByType NtAccessCheckByTypeAndAuditAlarm NtAccessCheckByTypeResultList NtAccessCheckByTypeResultListAndAuditAlarm NtAccessCheckByTypeResultListAndAuditAlarmByHandle NtAcquireProcessActivityReference NtAddAtom NtAddAtomEx NtAddBootEntry NtAddDriverEntry NtAdjustGroupsToken NtAdjustPrivilegesToken NtAdjustTokenClaimsAndDeviceGroups NtAlertResumeThread NtAlertThread NtAlertThreadByThreadId NtAllocateLocallyUniqueId NtAllocateReserveObject NtAllocateUserPhysicalPages NtAllocateUuids NtAllocateVirtualMemory NtAllocateVirtualMemoryEx NtAlpcAcceptConnectPort NtAlpcCancelMessage NtAlpcConnectPort NtAlpcConnectPortEx NtAlpcCreatePort NtAlpcCreatePortSection NtAlpcCreateResourceReserve NtAlpcCreateSectionView NtAlpcCreateSecurityContext NtAlpcDeletePortSection NtAlpcDeleteResourceReserve NtAlpcDeleteSectionView NtAlpcDeleteSecurityContext NtAlpcDisconnectPort NtAlpcImpersonateClientContainerOfPort NtAlpcImpersonateClientOfPort NtAlpcOpenSenderProcess NtAlpcOpenSenderThread NtAlpcQueryInformation NtAlpcQueryInformationMessage NtAlpcRevokeSecurityContext NtAlpcSendWaitReceivePort NtAlpcSetInformation NtApphelpCacheControl NtAreMappedFilesTheSame NtAssignProcessToJobObject NtAssociateWaitCompletionPacket NtCallEnclave NtCallbackReturn NtCancelIoFile NtCancelIoFileEx NtCancelSynchronousIoFile NtCancelTimer NtCancelTimer2 NtCancelWaitCompletionPacket NtClearEvent NtClose NtCloseObjectAuditAlarm NtCommitComplete NtCommitEnlistment NtCommitRegistryTransaction NtCommitTransaction NtCompactKeys NtCompareObjects NtCompareSigningLevels " \
"NtCompareTokens NtCompleteConnectPort NtCompressKey NtConnectPort NtContinue NtConvertBetweenAuxiliaryCounterAndPerformanceCounter NtCreateDebugObject NtCreateDirectoryObject NtCreateDirectoryObjectEx NtCreateEnclave NtCreateEnlistment NtCreateEvent NtCreateEventPair NtCreateFile NtCreateIRTimer NtCreateIoCompletion NtCreateJobObject NtCreateJobSet NtCreateKey NtCreateKeyTransacted NtCreateKeyedEvent NtCreateLowBoxToken NtCreateMailslotFile NtCreateMutant NtCreateNamedPipeFile NtCreatePagingFile NtCreatePartition NtCreatePort NtCreatePrivateNamespace NtCreateProcess NtCreateProcessEx NtCreateProfile NtCreateProfileEx NtCreateRegistryTransaction NtCreateResourceManager NtCreateSection NtCreateSemaphore NtCreateSymbolicLinkObject NtCreateThread NtCreateThreadEx NtCreateTimer NtCreateTimer2 NtCreateToken NtCreateTokenEx NtCreateTransaction NtCreateTransactionManager NtCreateUserProcess NtCreateWaitCompletionPacket NtCreateWaitablePort NtCreateWnfStateName NtCreateWorkerFactory NtDebugActiveProcess NtDebugContinue NtDelayExecution NtDeleteAtom NtDeleteBootEntry NtDeleteDriverEntry NtDeleteFile NtDeleteKey NtDeleteObjectAuditAlarm NtDeletePrivateNamespace NtDeleteValueKey NtDeleteWnfStateData NtDeleteWnfStateName NtDeviceIoControlFile NtDisableLastKnownGood NtDisplayString NtDrawText NtDuplicateObject NtDuplicateToken NtEnableLastKnownGood NtEnumerateBootEntries NtEnumerateDriverEntries NtEnumerateKey NtEnumerateSystemEnvironmentValuesEx NtEnumerateTransactionObject NtEnumerateValueKey NtExtendSection NtFilterBootOption NtFilterToken NtFilterTokenEx NtFindAtom NtFlushBuffersFile NtFlushBuffersFileEx NtFlushInstallUILanguage NtFlushInstructionCache NtFlushKey NtFlushProcessWriteBuffers NtFlushVirtualMemory NtFlushWriteBuffer NtFreeUserPhysicalPages NtFreeVirtualMemory NtFreezeRegistry NtFreezeTransactions NtFsControlFile NtGetCachedSigningLevel NtGetCompleteWnfStateSubscription NtGetContextThread NtGetCurrentProcessorNumber NtGetCurrentProcessorNumberEx NtGetDevicePowerState " \
"NtGetMUIRegistryInfo NtGetNextProcess NtGetNextThread NtGetNlsSectionPtr NtGetNotificationResourceManager NtGetTickCount NtGetWriteWatch NtImpersonateAnonymousToken NtImpersonateClientOfPort NtImpersonateThread NtInitializeEnclave NtInitializeNlsFiles NtInitializeRegistry NtInitiatePowerAction NtIsProcessInJob NtIsSystemResumeAutomatic NtIsUILanguageComitted NtListenPort NtLoadDriver NtLoadEnclaveData NtLoadHotPatch NtLoadKey NtLoadKey2 NtLoadKeyEx NtLockFile NtLockProductActivationKeys NtLockRegistryKey NtLockVirtualMemory NtMakePermanentObject NtMakeTemporaryObject NtManagePartition NtMapCMFModule NtMapUserPhysicalPages NtMapUserPhysicalPagesScatter NtMapViewOfSection NtMapViewOfSectionEx NtModifyBootEntry NtModifyDriverEntry NtNotifyChangeDirectoryFile NtNotifyChangeDirectoryFileEx NtNotifyChangeKey NtNotifyChangeMultipleKeys NtNotifyChangeSession NtOpenDirectoryObject NtOpenEnlistment NtOpenEvent NtOpenEventPair NtOpenFile NtOpenIoCompletion NtOpenJobObject NtOpenKey NtOpenKeyEx NtOpenKeyTransacted NtOpenKeyTransactedEx NtOpenKeyedEvent NtOpenMutant NtOpenObjectAuditAlarm NtOpenPartition NtOpenPrivateNamespace NtOpenProcess NtOpenProcessToken NtOpenProcessTokenEx NtOpenRegistryTransaction NtOpenResourceManager NtOpenSection NtOpenSemaphore NtOpenSession NtOpenSymbolicLinkObject NtOpenThread NtOpenThreadToken NtOpenThreadTokenEx NtOpenTimer NtOpenTransaction NtOpenTransactionManager NtPlugPlayControl NtPowerInformation NtPrePrepareComplete NtPrePrepareEnlistment NtPrepareComplete NtPrepareEnlistment NtPrivilegeCheck NtPrivilegeObjectAuditAlarm NtPrivilegedServiceAuditAlarm NtPropagationComplete NtPropagationFailed NtProtectVirtualMemory NtPulseEvent NtQueryAttributesFile NtQueryAuxiliaryCounterFrequency NtQueryBootEntryOrder NtQueryBootOptions NtQueryDebugFilterState NtQueryDefaultLocale NtQueryDefaultUILanguage NtQueryDirectoryFile NtQueryDirectoryFileEx NtQueryDirectoryObject NtQueryDriverEntryOrder NtQueryEaFile NtQueryEvent NtQueryFullAttributesFile NtQueryInformationAtom " \
"NtQueryInformationByName NtQueryInformationEnlistment NtQueryInformationFile NtQueryInformationJobObject NtQueryInformationPort NtQueryInformationProcess NtQueryInformationResourceManager NtQueryInformationThread NtQueryInformationToken NtQueryInformationTransaction NtQueryInformationTransactionManager NtQueryInformationWorkerFactory NtQueryInstallUILanguage NtQueryIntervalProfile NtQueryIoCompletion NtQueryKey NtQueryLicenseValue NtQueryMultipleValueKey NtQueryMutant NtQueryObject NtQueryOpenSubKeys NtQueryOpenSubKeysEx NtQueryPerformanceCounter NtQueryPortInformationProcess NtQueryQuotaInformationFile NtQuerySection NtQuerySecurityAttributesToken NtQuerySecurityObject NtQuerySecurityPolicy NtQuerySemaphore NtQuerySymbolicLinkObject NtQuerySystemEnvironmentValue NtQuerySystemEnvironmentValueEx NtQuerySystemInformation NtQuerySystemInformationEx NtQuerySystemTime NtQueryTimer NtQueryTimerResolution NtQueryValueKey NtQueryVirtualMemory NtQueryVolumeInformationFile NtQueryWnfStateData NtQueryWnfStateNameInformation NtQueueApcThread NtQueueApcThreadEx NtRaiseException NtRaiseHardError NtReadFile NtReadFileScatter NtReadOnlyEnlistment NtReadRequestData NtReadVirtualMemory NtRecoverEnlistment NtRecoverResourceManager NtRecoverTransactionManager NtRegisterProtocolAddressInformation NtRegisterThreadTerminatePort NtReleaseKeyedEvent NtReleaseMutant NtReleaseSemaphore NtReleaseWorkerFactoryWorker NtRemoveIoCompletion NtRemoveIoCompletionEx NtRemoveProcessDebug NtRenameKey NtRenameTransactionManager NtReplaceKey NtReplacePartitionUnit NtReplyPort NtReplyWaitReceivePort NtReplyWaitReceivePortEx NtReplyWaitReplyPort NtRequestPort NtRequestWaitReplyPort NtResetEvent NtResetWriteWatch NtRestoreKey NtResumeProcess NtResumeThread NtRevertContainerImpersonation NtRollbackComplete NtRollbackEnlistment NtRollbackRegistryTransaction NtRollbackTransaction NtRollforwardTransactionManager NtSaveKey NtSaveKeyEx NtSaveMergedKeys NtSecureConnectPort NtSerializeBoot NtSetBootEntryOrder NtSetBootOptions " \
"NtSetCachedSigningLevel NtSetCachedSigningLevel2 NtSetContextThread NtSetDebugFilterState NtSetDefaultHardErrorPort NtSetDefaultLocale NtSetDefaultUILanguage NtSetDriverEntryOrder NtSetEaFile NtSetEvent NtSetEventBoostPriority NtSetHighEventPair NtSetHighWaitLowEventPair NtSetIRTimer NtSetInformationDebugObject NtSetInformationEnlistment NtSetInformationFile NtSetInformationJobObject NtSetInformationKey NtSetInformationObject NtSetInformationProcess NtSetInformationResourceManager NtSetInformationSymbolicLink NtSetInformationThread NtSetInformationToken NtSetInformationTransaction NtSetInformationTransactionManager NtSetInformationVirtualMemory NtSetInformationWorkerFactory NtSetIntervalProfile NtSetIoCompletion NtSetIoCompletionEx NtSetLdtEntries NtSetLowEventPair NtSetLowWaitHighEventPair NtSetQuotaInformationFile NtSetSecurityObject NtSetSystemEnvironmentValue NtSetSystemEnvironmentValueEx NtSetSystemInformation NtSetSystemPowerState NtSetSystemTime NtSetThreadExecutionState NtSetTimer NtSetTimer2 NtSetTimerEx NtSetTimerResolution NtSetUuidSeed NtSetValueKey NtSetVolumeInformationFile NtSetWnfProcessNotificationEvent NtShutdownSystem NtShutdownWorkerFactory NtSignalAndWaitForSingleObject NtSinglePhaseReject NtStartProfile NtStopProfile NtSubscribeWnfStateChange NtSuspendProcess NtSuspendThread NtSystemDebugControl NtTerminateEnclave NtTerminateJobObject NtTerminateProcess NtTerminateThread NtTestAlert NtThawRegistry NtThawTransactions NtTraceControl NtTraceEvent NtTranslateFilePath NtUmsThreadYield NtUnloadDriver NtUnloadKey NtUnloadKey2 NtUnloadKeyEx NtUnlockFile NtUnlockVirtualMemory NtUnmapViewOfSection NtUnmapViewOfSectionEx NtUnsubscribeWnfStateChange NtUpdateWnfStateData NtVdmControl NtWaitForAlertByThreadId NtWaitForDebugEvent NtWaitForKeyedEvent NtWaitForMultipleObjects NtWaitForMultipleObjects32 NtWaitForSingleObject NtWaitForWorkViaWorkerFactory NtWaitHighEventPair NtWaitLowEventPair NtWorkerFactoryWorkerReady NtWriteFile NtWriteFileGather NtWriteRequestData " \
"NtWriteVirtualMemory NtYieldExecution NtdllDefWindowProc_A NtdllDefWindowProc_W NtdllDialogWndProc_A NtdllDialogWndProc_W PfxFindPrefix PfxInitialize PfxInsertPrefix PfxRemovePrefix PssNtCaptureSnapshot PssNtDuplicateSnapshot PssNtFreeRemoteSnapshot PssNtFreeSnapshot PssNtFreeWalkMarker PssNtQuerySnapshot PssNtValidateDescriptor PssNtWalkSnapshot RtlAbortRXact RtlAbsoluteToSelfRelativeSD RtlAcquirePebLock RtlAcquirePrivilege RtlAcquireReleaseSRWLockExclusive RtlAcquireResourceExclusive RtlAcquireResourceShared RtlAcquireSRWLockExclusive RtlAcquireSRWLockShared RtlActivateActivationContext RtlActivateActivationContextEx RtlActivateActivationContextUnsafeFast RtlAddAccessAllowedAce RtlAddAccessAllowedAceEx RtlAddAccessAllowedObjectAce RtlAddAccessDeniedAce RtlAddAccessDeniedAceEx RtlAddAccessDeniedObjectAce RtlAddAccessFilterAce RtlAddAce RtlAddActionToRXact RtlAddAtomToAtomTable RtlAddAttributeActionToRXact RtlAddAuditAccessAce RtlAddAuditAccessAceEx RtlAddAuditAccessObjectAce RtlAddCompoundAce RtlAddFunctionTable RtlAddGrowableFunctionTable RtlAddIntegrityLabelToBoundaryDescriptor RtlAddMandatoryAce RtlAddProcessTrustLabelAce RtlAddRefActivationContext RtlAddRefMemoryStream RtlAddResourceAttributeAce RtlAddSIDToBoundaryDescriptor RtlAddScopedPolicyIDAce RtlAddVectoredContinueHandler RtlAddVectoredExceptionHandler RtlAddressInSectionTable RtlAdjustPrivilege RtlAllocateActivationContextStack RtlAllocateAndInitializeSid RtlAllocateAndInitializeSidEx RtlAllocateHandle RtlAllocateHeap RtlAllocateMemoryBlockLookaside RtlAllocateMemoryZone RtlAllocateWnfSerializationGroup RtlAnsiCharToUnicodeChar RtlAnsiStringToUnicodeSize RtlAnsiStringToUnicodeString RtlAppendAsciizToString RtlAppendPathElement RtlAppendStringToString RtlAppendUnicodeStringToString RtlAppendUnicodeToString RtlApplicationVerifierStop RtlApplyRXact RtlApplyRXactNoFlush RtlAppxIsFileOwnedByTrustedInstaller RtlAreAllAccessesGranted RtlAreAnyAccessesGranted RtlAreBitsClear RtlAreBitsSet RtlAreLongPathsEnabled RtlAssert " \
"RtlAvlInsertNodeEx RtlAvlRemoveNode RtlBarrier RtlBarrierForDelete RtlCallEnclaveReturn RtlCancelTimer RtlCanonicalizeDomainName RtlCapabilityCheck RtlCapabilityCheckForSingleSessionSku RtlCaptureContext RtlCaptureStackBackTrace RtlCharToInteger RtlCheckBootStatusIntegrity RtlCheckForOrphanedCriticalSections RtlCheckPortableOperatingSystem RtlCheckRegistryKey RtlCheckSandboxedToken RtlCheckSystemBootStatusIntegrity RtlCheckTokenCapability RtlCheckTokenMembership RtlCheckTokenMembershipEx RtlCleanUpTEBLangLists RtlClearAllBits RtlClearBit RtlClearBits RtlClearThreadWorkOnBehalfTicket RtlCloneMemoryStream RtlCloneUserProcess RtlCmDecodeMemIoResource RtlCmEncodeMemIoResource RtlCommitDebugInfo RtlCommitMemoryStream RtlCompactHeap RtlCompareAltitudes RtlCompareMemory RtlCompareMemoryUlong RtlCompareString RtlCompareUnicodeString RtlCompareUnicodeStrings RtlCompleteProcessCloning RtlCompressBuffer RtlComputeCrc32 RtlComputeImportTableHash RtlComputePrivatizedDllName_U RtlConnectToSm RtlConsoleMultiByteToUnicodeN RtlContractHashTable RtlConvertDeviceFamilyInfoToString RtlConvertExclusiveToShared RtlConvertLCIDToString RtlConvertSRWLockExclusiveToShared RtlConvertSharedToExclusive RtlConvertSidToUnicodeString RtlConvertToAutoInheritSecurityObject RtlCopyBitMap RtlCopyContext RtlCopyExtendedContext RtlCopyLuid RtlCopyLuidAndAttributesArray RtlCopyMappedMemory RtlCopyMemory RtlCopyMemoryNonTemporal RtlCopyMemoryStreamTo RtlCopyOutOfProcessMemoryStreamTo RtlCopySecurityDescriptor RtlCopySid RtlCopySidAndAttributesArray RtlCopyString RtlCopyUnicodeString RtlCrc32 RtlCrc64 RtlCreateAcl RtlCreateActivationContext RtlCreateAndSetSD RtlCreateAtomTable RtlCreateBootStatusDataFile RtlCreateBoundaryDescriptor RtlCreateEnvironment RtlCreateEnvironmentEx RtlCreateHashTable RtlCreateHashTableEx RtlCreateHeap RtlCreateMemoryBlockLookaside RtlCreateMemoryZone RtlCreateProcessParameters RtlCreateProcessParametersEx RtlCreateProcessReflection RtlCreateQueryDebugBuffer RtlCreateRegistryKey RtlCreateSecurityDescriptor " \
"RtlCreateServiceSid RtlCreateSystemVolumeInformationFolder RtlCreateTagHeap RtlCreateTimer RtlCreateTimerQueue RtlCreateUmsCompletionList RtlCreateUmsThreadContext RtlCreateUnicodeString RtlCreateUnicodeStringFromAsciiz RtlCreateUserProcess RtlCreateUserProcessEx RtlCreateUserSecurityObject RtlCreateUserStack RtlCreateUserThread RtlCreateVirtualAccountSid RtlCultureNameToLCID RtlCustomCPToUnicodeN RtlCutoverTimeToSystemTime RtlDeCommitDebugInfo RtlDeNormalizeProcessParams RtlDeactivateActivationContext RtlDeactivateActivationContextUnsafeFast RtlDebugPrintTimes RtlDecodePointer RtlDecodeRemotePointer RtlDecodeSystemPointer RtlDecompressBuffer RtlDecompressBufferEx RtlDecompressFragment RtlDefaultNpAcl RtlDelete RtlDeleteAce RtlDeleteAtomFromAtomTable RtlDeleteBarrier RtlDeleteBoundaryDescriptor RtlDeleteCriticalSection RtlDeleteElementGenericTable RtlDeleteElementGenericTableAvl RtlDeleteElementGenericTableAvlEx RtlDeleteFunctionTable RtlDeleteGrowableFunctionTable RtlDeleteHashTable RtlDeleteNoSplay RtlDeleteRegistryValue RtlDeleteResource RtlDeleteSecurityObject RtlDeleteTimer RtlDeleteTimerQueue RtlDeleteTimerQueueEx RtlDeleteUmsCompletionList RtlDeleteUmsThreadContext RtlDequeueUmsCompletionListItems RtlDeregisterSecureMemoryCacheCallback RtlDeregisterWait RtlDeregisterWaitEx RtlDeriveCapabilitySidsFromName RtlDestroyAtomTable RtlDestroyEnvironment RtlDestroyHandleTable RtlDestroyHeap RtlDestroyMemoryBlockLookaside RtlDestroyMemoryZone RtlDestroyProcessParameters RtlDestroyQueryDebugBuffer RtlDetectHeapLeaks RtlDetermineDosPathNameType_U RtlDisableThreadProfiling RtlDllShutdownInProgress RtlDnsHostNameToComputerName RtlDoesFileExists_U RtlDosApplyFileIsolationRedirection_Ustr RtlDosLongPathNameToNtPathName_U_WithStatus RtlDosLongPathNameToRelativeNtPathName_U_WithStatus RtlDosPathNameToNtPathName_U RtlDosPathNameToNtPathName_U_WithStatus RtlDosPathNameToRelativeNtPathName_U RtlDosPathNameToRelativeNtPathName_U_WithStatus RtlDosSearchPath_U RtlDosSearchPath_Ustr RtlDowncaseUnicodeChar " \
"RtlDowncaseUnicodeString RtlDrainNonVolatileFlush RtlDumpResource RtlDuplicateUnicodeString RtlEmptyAtomTable RtlEnableEarlyCriticalSectionEventCreation RtlEnableThreadProfiling RtlEnclaveCallDispatch RtlEnclaveCallDispatchReturn RtlEncodePointer RtlEncodeRemotePointer RtlEncodeSystemPointer RtlEndEnumerationHashTable RtlEndStrongEnumerationHashTable RtlEndWeakEnumerationHashTable RtlEnterCriticalSection RtlEnterUmsSchedulingMode RtlEnumProcessHeaps RtlEnumerateEntryHashTable RtlEnumerateGenericTable RtlEnumerateGenericTableAvl RtlEnumerateGenericTableLikeADirectory RtlEnumerateGenericTableWithoutSplaying RtlEnumerateGenericTableWithoutSplayingAvl RtlEqualComputerName RtlEqualDomainName RtlEqualLuid RtlEqualPrefixSid RtlEqualSid RtlEqualString RtlEqualUnicodeString RtlEqualWnfChangeStamps RtlEraseUnicodeString RtlEthernetAddressToStringA RtlEthernetAddressToStringW RtlEthernetStringToAddressA RtlEthernetStringToAddressW RtlExecuteUmsThread RtlExitUserProcess RtlExitUserThread RtlExpandEnvironmentStrings RtlExpandEnvironmentStrings_U RtlExpandHashTable RtlExtendCorrelationVector RtlExtendMemoryBlockLookaside RtlExtendMemoryZone RtlExtractBitMap RtlFillMemory RtlFinalReleaseOutOfProcessMemoryStream RtlFindAceByType RtlFindActivationContextSectionGuid RtlFindActivationContextSectionString RtlFindCharInUnicodeString RtlFindClearBits RtlFindClearBitsAndSet RtlFindClearRuns RtlFindClosestEncodableLength RtlFindExportedRoutineByName RtlFindLastBackwardRunClear RtlFindLeastSignificantBit RtlFindLongestRunClear RtlFindMessage RtlFindMostSignificantBit RtlFindNextForwardRunClear RtlFindSetBits RtlFindSetBitsAndClear RtlFindUnicodeSubstring RtlFirstEntrySList RtlFirstFreeAce RtlFlsAlloc RtlFlsFree RtlFlushHeaps RtlFlushNonVolatileMemory RtlFlushNonVolatileMemoryRanges RtlFlushSecureMemoryCache RtlFormatCurrentUserKeyPath RtlFormatMessage RtlFormatMessageEx RtlFreeActivationContextStack RtlFreeAnsiString RtlFreeHandle RtlFreeHeap RtlFreeMemoryBlockLookaside RtlFreeNonVolatileToken " \
"RtlFreeOemString RtlFreeSid RtlFreeThreadActivationContextStack RtlFreeUnicodeString RtlFreeUserStack RtlGUIDFromString RtlGenerate8dot3Name RtlGetAce RtlGetActiveActivationContext RtlGetActiveConsoleId RtlGetAppContainerNamedObjectPath RtlGetAppContainerParent RtlGetAppContainerSidType RtlGetCallersAddress RtlGetCompressionWorkSpaceSize RtlGetConsoleSessionForegroundProcessId RtlGetControlSecurityDescriptor RtlGetCriticalSectionRecursionCount RtlGetCurrentDirectory_U RtlGetCurrentPeb RtlGetCurrentProcessorNumber RtlGetCurrentProcessorNumberEx RtlGetCurrentServiceSessionId RtlGetCurrentTransaction RtlGetCurrentUmsThread RtlGetDaclSecurityDescriptor RtlGetDeviceFamilyInfoEnum RtlGetElementGenericTable RtlGetElementGenericTableAvl RtlGetEnabledExtendedFeatures RtlGetExePath RtlGetExtendedContextLength RtlGetExtendedFeaturesMask RtlGetFileMUIPath RtlGetFrame RtlGetFullPathName_U RtlGetFullPathName_UEx RtlGetFullPathName_UstrEx RtlGetFunctionTableListHead RtlGetGroupSecurityDescriptor RtlGetIntegerAtom RtlGetInterruptTimePrecise RtlGetLastNtStatus RtlGetLastWin32Error RtlGetLengthWithoutLastFullDosOrNtPathElement RtlGetLengthWithoutTrailingPathSeperators RtlGetLocaleFileMappingAddress RtlGetLongestNtPathLength RtlGetNativeSystemInformation RtlGetNextEntryHashTable RtlGetNextUmsListItem RtlGetNonVolatileToken RtlGetNtGlobalFlags RtlGetNtProductType RtlGetNtSystemRoot RtlGetNtVersionNumbers RtlGetOwnerSecurityDescriptor RtlGetParentLocaleName RtlGetPersistedStateLocation RtlGetProcessHeaps RtlGetProcessPreferredUILanguages RtlGetProductInfo RtlGetSaclSecurityDescriptor RtlGetSearchPath RtlGetSecurityDescriptorRMControl RtlGetSessionProperties RtlGetSetBootStatusData RtlGetSuiteMask RtlGetSystemBootStatus RtlGetSystemBootStatusEx RtlGetSystemPreferredUILanguages RtlGetSystemTimePrecise RtlGetThreadErrorMode RtlGetThreadLangIdByIndex RtlGetThreadPreferredUILanguages RtlGetThreadWorkOnBehalfTicket RtlGetTokenNamedObjectPath RtlGetUILanguageInfo RtlGetUmsCompletionListEvent RtlGetUnloadEventTrace " \
"RtlGetUnloadEventTraceEx RtlGetUserInfoHeap RtlGetUserPreferredUILanguages RtlGetVersion RtlGrowFunctionTable RtlGuardCheckLongJumpTarget RtlHashUnicodeString RtlHeapTrkInitialize RtlIdentifierAuthoritySid RtlIdnToAscii RtlIdnToNameprepUnicode RtlIdnToUnicode RtlImageDirectoryEntryToData RtlImageNtHeader RtlImageNtHeaderEx RtlImageRvaToSection RtlImageRvaToVa RtlImpersonateSelf RtlImpersonateSelfEx RtlIncrementCorrelationVector RtlInitAnsiString RtlInitAnsiStringEx RtlInitBarrier RtlInitCodePageTable RtlInitEnumerationHashTable RtlInitMemoryStream RtlInitNlsTables RtlInitOutOfProcessMemoryStream RtlInitString RtlInitStringEx RtlInitStrongEnumerationHashTable RtlInitUnicodeString RtlInitUnicodeStringEx RtlInitWeakEnumerationHashTable RtlInitializeAtomPackage RtlInitializeBitMap RtlInitializeBitMapEx RtlInitializeConditionVariable RtlInitializeContext RtlInitializeCorrelationVector RtlInitializeCriticalSection RtlInitializeCriticalSectionAndSpinCount RtlInitializeCriticalSectionEx RtlInitializeExtendedContext RtlInitializeGenericTable RtlInitializeGenericTableAvl RtlInitializeHandleTable RtlInitializeNtUserPfn RtlInitializeRXact RtlInitializeResource RtlInitializeSListHead RtlInitializeSRWLock RtlInitializeSid RtlInitializeSidEx RtlInsertElementGenericTable RtlInsertElementGenericTableAvl RtlInsertElementGenericTableFull RtlInsertElementGenericTableFullAvl RtlInsertEntryHashTable RtlInstallFunctionTableCallback RtlInt64ToUnicodeString RtlIntegerToChar RtlIntegerToUnicodeString RtlInterlockedClearBitRun RtlInterlockedFlushSList RtlInterlockedPopEntrySList RtlInterlockedPushEntrySList RtlInterlockedPushListSList RtlInterlockedPushListSListEx RtlInterlockedSetBitRun RtlIoDecodeMemIoResource RtlIoEncodeMemIoResource RtlIpv4AddressToStringA RtlIpv4AddressToStringExA RtlIpv4AddressToStringExW RtlIpv4AddressToStringW RtlIpv4StringToAddressA RtlIpv4StringToAddressExA RtlIpv4StringToAddressExW RtlIpv4StringToAddressW RtlIpv6AddressToStringA RtlIpv6AddressToStringExA RtlIpv6AddressToStringExW " \
"RtlIpv6AddressToStringW RtlIpv6StringToAddressA RtlIpv6StringToAddressExA RtlIpv6StringToAddressExW RtlIpv6StringToAddressW RtlIsActivationContextActive RtlIsCapabilitySid RtlIsCloudFilesPlaceholder RtlIsCriticalSectionLocked RtlIsCriticalSectionLockedByThread RtlIsCurrentProcess RtlIsCurrentThread RtlIsCurrentThreadAttachExempt RtlIsDosDeviceName_U RtlIsElevatedRid RtlIsGenericTableEmpty RtlIsGenericTableEmptyAvl RtlIsMultiSessionSku RtlIsMultiUsersInSessionSku RtlIsNameInExpression RtlIsNameInUnUpcasedExpression RtlIsNameLegalDOS8Dot3 RtlIsNonEmptyDirectoryReparsePointAllowed RtlIsNormalizedString RtlIsPackageSid RtlIsParentOfChildAppContainer RtlIsPartialPlaceholder RtlIsPartialPlaceholderFileHandle RtlIsPartialPlaceholderFileInfo RtlIsProcessorFeaturePresent RtlIsStateSeparationEnabled RtlIsTextUnicode RtlIsThreadWithinLoaderCallout RtlIsUntrustedObject RtlIsValidHandle RtlIsValidIndexHandle RtlIsValidLocaleName RtlIsValidProcessTrustLabelSid RtlKnownExceptionFilter RtlLCIDToCultureName RtlLargeIntegerToChar RtlLcidToLocaleName RtlLeaveCriticalSection RtlLengthRequiredSid RtlLengthSecurityDescriptor RtlLengthSid RtlLengthSidAsUnicodeString RtlLoadString RtlLocalTimeToSystemTime RtlLocaleNameToLcid RtlLocateExtendedFeature RtlLocateExtendedFeature2 RtlLocateLegacyContext RtlLockBootStatusData RtlLockCurrentThread RtlLockHeap RtlLockMemoryBlockLookaside RtlLockMemoryStreamRegion RtlLockMemoryZone RtlLockModuleSection RtlLogStackBackTrace RtlLookupAtomInAtomTable RtlLookupElementGenericTable RtlLookupElementGenericTableAvl RtlLookupElementGenericTableFull RtlLookupElementGenericTableFullAvl RtlLookupEntryHashTable RtlLookupFirstMatchingElementGenericTableAvl RtlLookupFunctionEntry RtlLookupFunctionTable RtlMakeSelfRelativeSD RtlMapGenericMask RtlMapSecurityErrorToNtStatus RtlMoveMemory RtlMultiAppendUnicodeStringBuffer RtlMultiByteToUnicodeN RtlMultiByteToUnicodeSize RtlMultipleAllocateHeap RtlMultipleFreeHeap RtlNewInstanceSecurityObject RtlNewSecurityGrantedAccess " \
"RtlNewSecurityObject RtlNewSecurityObjectEx RtlNewSecurityObjectWithMultipleInheritance RtlNormalizeProcessParams RtlNormalizeString RtlNtPathNameToDosPathName RtlNtStatusToDosError RtlNtStatusToDosErrorNoTeb RtlNtdllName RtlNumberGenericTableElements RtlNumberGenericTableElementsAvl RtlNumberOfClearBits RtlNumberOfClearBitsInRange RtlNumberOfSetBits RtlNumberOfSetBitsInRange RtlNumberOfSetBitsUlongPtr RtlOemStringToUnicodeSize RtlOemStringToUnicodeString RtlOemToUnicodeN RtlOpenCurrentUser RtlOsDeploymentState RtlOwnerAcesPresent RtlPcToFileHeader RtlPinAtomInAtomTable RtlPopFrame RtlPrefixString RtlPrefixUnicodeString RtlPrepareForProcessCloning RtlProcessFlsData RtlProtectHeap RtlPublishWnfStateData RtlPushFrame RtlQueryActivationContextApplicationSettings RtlQueryAtomInAtomTable RtlQueryCriticalSectionOwner RtlQueryDepthSList RtlQueryDynamicTimeZoneInformation RtlQueryElevationFlags RtlQueryEnvironmentVariable RtlQueryEnvironmentVariable_U RtlQueryHeapInformation RtlQueryImageMitigationPolicy RtlQueryInformationAcl RtlQueryInformationActivationContext RtlQueryInformationActiveActivationContext RtlQueryInterfaceMemoryStream RtlQueryModuleInformation RtlQueryPackageClaims RtlQueryPackageIdentity RtlQueryPackageIdentityEx RtlQueryPerformanceCounter RtlQueryPerformanceFrequency RtlQueryProcessBackTraceInformation RtlQueryProcessDebugInformation RtlQueryProcessHeapInformation RtlQueryProcessLockInformation RtlQueryProcessPlaceholderCompatibilityMode RtlQueryProtectedPolicy RtlQueryRegistryValueWithFallback RtlQueryRegistryValues RtlQueryRegistryValuesEx RtlQueryResourcePolicy RtlQuerySecurityObject RtlQueryTagHeap RtlQueryThreadPlaceholderCompatibilityMode RtlQueryThreadProfiling RtlQueryTimeZoneInformation RtlQueryTokenHostIdAsUlong64 RtlQueryUmsThreadInformation RtlQueryUnbiasedInterruptTime RtlQueryValidationRunlevel RtlQueryWnfMetaNotification RtlQueryWnfStateData RtlQueryWnfStateDataWithExplicitScope RtlQueueApcWow64Thread RtlQueueWorkItem RtlRaiseCustomSystemEventTrigger " \
"RtlRaiseException RtlRaiseStatus RtlRandom RtlRandomEx RtlRbInsertNodeEx RtlRbRemoveNode RtlReAllocateHeap RtlReadMemoryStream RtlReadOutOfProcessMemoryStream RtlReadThreadProfilingData RtlRealPredecessor RtlRealSuccessor RtlRegisterForWnfMetaNotification RtlRegisterSecureMemoryCacheCallback RtlRegisterThreadWithCsrss RtlRegisterWait RtlReleaseActivationContext RtlReleaseMemoryStream RtlReleasePath RtlReleasePebLock RtlReleasePrivilege RtlReleaseRelativeName RtlReleaseResource RtlReleaseSRWLockExclusive RtlReleaseSRWLockShared RtlRemoteCall RtlRemoveEntryHashTable RtlRemovePrivileges RtlRemoveVectoredContinueHandler RtlRemoveVectoredExceptionHandler RtlReplaceSidInSd RtlReplaceSystemDirectoryInPath RtlReportException RtlReportExceptionEx RtlReportSilentProcessExit RtlReportSqmEscalation RtlResetMemoryBlockLookaside RtlResetMemoryZone RtlResetNtUserPfn RtlResetRtlTranslations RtlRestoreBootStatusDefaults RtlRestoreContext RtlRestoreLastWin32Error RtlRestoreSystemBootStatusDefaults RtlRetrieveNtUserPfn RtlRevertMemoryStream RtlRunDecodeUnicodeString RtlRunEncodeUnicodeString RtlRunOnceBeginInitialize RtlRunOnceComplete RtlRunOnceExecuteOnce RtlRunOnceInitialize RtlSecondsSince1970ToTime RtlSecondsSince1980ToTime RtlSeekMemoryStream RtlSelfRelativeToAbsoluteSD RtlSelfRelativeToAbsoluteSD2 RtlSendMsgToSm RtlSetAllBits RtlSetAttributesSecurityDescriptor RtlSetBit RtlSetBits RtlSetControlSecurityDescriptor RtlSetCriticalSectionSpinCount RtlSetCurrentDirectory_U RtlSetCurrentEnvironment RtlSetCurrentTransaction RtlSetDaclSecurityDescriptor RtlSetDynamicTimeZoneInformation RtlSetEnvironmentStrings RtlSetEnvironmentVar RtlSetEnvironmentVariable RtlSetExtendedFeaturesMask RtlSetGroupSecurityDescriptor RtlSetHeapInformation RtlSetImageMitigationPolicy RtlSetInformationAcl RtlSetIoCompletionCallback RtlSetLastWin32Error RtlSetLastWin32ErrorAndNtStatusFromNtStatus RtlSetMemoryStreamSize RtlSetOwnerSecurityDescriptor RtlSetPortableOperatingSystem RtlSetProcessDebugInformation RtlSetProcessIsCritical " \
"RtlSetProcessPlaceholderCompatibilityMode RtlSetProcessPreferredUILanguages RtlSetProtectedPolicy RtlSetProxiedProcessId RtlSetSaclSecurityDescriptor RtlSetSearchPathMode RtlSetSecurityDescriptorRMControl RtlSetSecurityObject RtlSetSecurityObjectEx RtlSetSystemBootStatus RtlSetSystemBootStatusEx RtlSetThreadErrorMode RtlSetThreadIsCritical RtlSetThreadPlaceholderCompatibilityMode RtlSetThreadPoolStartFunc RtlSetThreadPreferredUILanguages RtlSetThreadSubProcessTag RtlSetThreadWorkOnBehalfTicket RtlSetTimeZoneInformation RtlSetTimer RtlSetUmsThreadInformation RtlSetUnhandledExceptionFilter RtlSetUserFlagsHeap RtlSetUserValueHeap RtlSidDominates RtlSidDominatesForTrust RtlSidEqualLevel RtlSidHashInitialize RtlSidHashLookup RtlSidIsHigherLevel RtlSizeHeap RtlSleepConditionVariableCS RtlSleepConditionVariableSRW RtlSplay RtlStartRXact RtlStatMemoryStream RtlStringFromGUID RtlStringFromGUIDEx RtlStronglyEnumerateEntryHashTable RtlSubAuthorityCountSid RtlSubAuthoritySid RtlSubscribeWnfStateChangeNotification RtlSubtreePredecessor RtlSubtreeSuccessor RtlSwitchedVVI RtlSystemTimeToLocalTime RtlTestAndPublishWnfStateData RtlTestBit RtlTestBitEx RtlTestProtectedAccess RtlTimeFieldsToTime RtlTimeToElapsedTimeFields RtlTimeToSecondsSince1970 RtlTimeToSecondsSince1980 RtlTimeToTimeFields RtlTraceDatabaseAdd RtlTraceDatabaseCreate RtlTraceDatabaseDestroy RtlTraceDatabaseEnumerate RtlTraceDatabaseFind RtlTraceDatabaseLock RtlTraceDatabaseUnlock RtlTraceDatabaseValidate RtlTryAcquirePebLock RtlTryAcquireSRWLockExclusive RtlTryAcquireSRWLockShared RtlTryConvertSRWLockSharedToExclusiveOrRelease RtlTryEnterCriticalSection RtlUTF8ToUnicodeN RtlUmsThreadYield RtlUnhandledExceptionFilter RtlUnhandledExceptionFilter2 RtlUnicodeStringToAnsiSize RtlUnicodeStringToAnsiString RtlUnicodeStringToCountedOemString RtlUnicodeStringToInteger RtlUnicodeStringToOemSize RtlUnicodeStringToOemString RtlUnicodeToCustomCPN RtlUnicodeToMultiByteN RtlUnicodeToMultiByteSize RtlUnicodeToOemN RtlUnicodeToUTF8N RtlUniform " \
"RtlUnlockBootStatusData RtlUnlockCurrentThread RtlUnlockHeap RtlUnlockMemoryBlockLookaside RtlUnlockMemoryStreamRegion RtlUnlockMemoryZone RtlUnlockModuleSection RtlUnsubscribeWnfNotificationWaitForCompletion RtlUnsubscribeWnfNotificationWithCompletionCallback RtlUnsubscribeWnfStateChangeNotification RtlUnwind RtlUnwindEx RtlUpcaseUnicodeChar RtlUpcaseUnicodeString RtlUpcaseUnicodeStringToAnsiString RtlUpcaseUnicodeStringToCountedOemString RtlUpcaseUnicodeStringToOemString RtlUpcaseUnicodeToCustomCPN RtlUpcaseUnicodeToMultiByteN RtlUpcaseUnicodeToOemN RtlUpdateClonedCriticalSection RtlUpdateClonedSRWLock RtlUpdateTimer RtlUpperChar RtlUpperString RtlUserThreadStart RtlValidAcl RtlValidProcessProtection RtlValidRelativeSecurityDescriptor RtlValidSecurityDescriptor RtlValidSid RtlValidateCorrelationVector RtlValidateHeap RtlValidateProcessHeaps RtlValidateUnicodeString RtlVerifyVersionInfo RtlVirtualUnwind RtlWaitForWnfMetaNotification RtlWaitOnAddress RtlWakeAddressAll RtlWakeAddressAllNoFence RtlWakeAddressSingle RtlWakeAddressSingleNoFence RtlWakeAllConditionVariable RtlWakeConditionVariable RtlWalkFrameChain RtlWalkHeap RtlWeaklyEnumerateEntryHashTable RtlWerpReportException RtlWnfCompareChangeStamp RtlWnfDllUnloadCallback RtlWow64CallFunction64 RtlWow64EnableFsRedirection RtlWow64EnableFsRedirectionEx RtlWow64GetCpuAreaInfo RtlWow64GetCurrentCpuArea RtlWow64GetCurrentMachine RtlWow64GetEquivalentMachineCHPE RtlWow64GetProcessMachines RtlWow64GetSharedInfoProcess RtlWow64GetThreadContext RtlWow64GetThreadSelectorEntry RtlWow64IsWowGuestMachineSupported RtlWow64LogMessageInEventLogger RtlWow64PopAllCrossProcessWork RtlWow64PopCrossProcessWork RtlWow64PushCrossProcessWork RtlWow64SetThreadContext RtlWow64SuspendThread RtlWriteMemoryStream RtlWriteNonVolatileMemory RtlWriteRegistryValue RtlZeroHeap RtlZeroMemory RtlZombifyActivationContext RtlpApplyLengthFunction RtlpCheckDynamicTimeZoneInformation RtlpCleanupRegistryKeys RtlpConvertAbsoluteToRelativeSecurityAttribute " \
"RtlpConvertCultureNamesToLCIDs RtlpConvertLCIDsToCultureNames RtlpConvertRelativeToAbsoluteSecurityAttribute RtlpCreateProcessRegistryInfo RtlpEnsureBufferSize RtlpExecuteUmsThread RtlpFreezeTimeBias RtlpGetDeviceFamilyInfoEnum RtlpGetLCIDFromLangInfoNode RtlpGetNameFromLangInfoNode RtlpGetSystemDefaultUILanguage RtlpGetUserOrMachineUILanguage4NLS RtlpInitializeLangRegistryInfo RtlpIsQualifiedLanguage RtlpLoadMachineUIByPolicy RtlpLoadUserUIByPolicy RtlpMergeSecurityAttributeInformation RtlpMuiFreeLangRegistryInfo RtlpMuiRegCreateRegistryInfo RtlpMuiRegFreeRegistryInfo RtlpMuiRegLoadRegistryInfo RtlpNotOwnerCriticalSection RtlpNtCreateKey RtlpNtEnumerateSubKey RtlpNtMakeTemporaryKey RtlpNtOpenKey RtlpNtQueryValueKey RtlpNtSetValueKey RtlpQueryDefaultUILanguage RtlpQueryProcessDebugInformationFromWow64 RtlpQueryProcessDebugInformationRemote RtlpRefreshCachedUILanguage RtlpSetInstallLanguage RtlpSetPreferredUILanguages RtlpSetUserPreferredUILanguages RtlpUmsExecuteYieldThreadEnd RtlpUmsThreadYield RtlpUnWaitCriticalSection RtlpVerifyAndCommitUILanguageSettings RtlpWaitForCriticalSection RtlxAnsiStringToUnicodeSize RtlxOemStringToUnicodeSize RtlxUnicodeStringToAnsiSize RtlxUnicodeStringToOemSize SbExecuteProcedure SbSelectProcedure ShipAssert ShipAssertGetBufferInfo ShipAssertMsgA ShipAssertMsgW TpAllocAlpcCompletion TpAllocAlpcCompletionEx TpAllocCleanupGroup TpAllocIoCompletion TpAllocJobNotification TpAllocPool TpAllocTimer TpAllocWait TpAllocWork TpAlpcRegisterCompletionList TpAlpcUnregisterCompletionList TpCallbackDetectedUnrecoverableError TpCallbackIndependent TpCallbackLeaveCriticalSectionOnCompletion TpCallbackMayRunLong TpCallbackReleaseMutexOnCompletion TpCallbackReleaseSemaphoreOnCompletion TpCallbackSendAlpcMessageOnCompletion TpCallbackSendPendingAlpcMessage TpCallbackSetEventOnCompletion TpCallbackUnloadDllOnCompletion TpCancelAsyncIoOperation TpCaptureCaller TpCheckTerminateWorker TpDbgDumpHeapUsage TpDbgSetLogRoutine TpDisablePoolCallbackChecks TpDisassociateCallback " \
"TpIsTimerSet TpPostWork TpQueryPoolStackInformation TpReleaseAlpcCompletion TpReleaseCleanupGroup TpReleaseCleanupGroupMembers TpReleaseIoCompletion TpReleaseJobNotification TpReleasePool TpReleaseTimer TpReleaseWait TpReleaseWork TpSetDefaultPoolMaxThreads TpSetDefaultPoolStackInformation TpSetPoolMaxThreads TpSetPoolMaxThreadsSoftLimit TpSetPoolMinThreads TpSetPoolStackInformation TpSetPoolThreadBasePriority TpSetPoolWorkerThreadIdleTimeout TpSetTimer TpSetTimerEx TpSetWait TpSetWaitEx TpSimpleTryPost TpStartAsyncIoOperation TpTimerOutstandingCallbackCount TpTrimPools TpWaitForAlpcCompletion TpWaitForIoCompletion TpWaitForJobNotification TpWaitForTimer TpWaitForWait TpWaitForWork VerSetConditionMask WerReportExceptionWorker WerReportSQMEvent WinSqmAddToAverageDWORD WinSqmAddToStream WinSqmAddToStreamEx WinSqmCheckEscalationAddToStreamEx WinSqmCheckEscalationSetDWORD WinSqmCheckEscalationSetDWORD64 WinSqmCheckEscalationSetString WinSqmCommonDatapointDelete WinSqmCommonDatapointSetDWORD WinSqmCommonDatapointSetDWORD64 WinSqmCommonDatapointSetStreamEx WinSqmCommonDatapointSetString WinSqmEndSession WinSqmEventEnabled WinSqmEventWrite WinSqmGetEscalationRuleStatus WinSqmGetInstrumentationProperty WinSqmIncrementDWORD WinSqmIsOptedIn WinSqmIsOptedInEx WinSqmIsSessionDisabled WinSqmSetDWORD WinSqmSetDWORD64 WinSqmSetEscalationInfo WinSqmSetIfMaxDWORD WinSqmSetIfMinDWORD WinSqmSetString WinSqmStartSession WinSqmStartSessionForPartner WinSqmStartSqmOptinListener ZwAcceptConnectPort ZwAccessCheck ZwAccessCheckAndAuditAlarm ZwAccessCheckByType ZwAccessCheckByTypeAndAuditAlarm ZwAccessCheckByTypeResultList ZwAccessCheckByTypeResultListAndAuditAlarm ZwAccessCheckByTypeResultListAndAuditAlarmByHandle ZwAcquireProcessActivityReference ZwAddAtom ZwAddAtomEx ZwAddBootEntry ZwAddDriverEntry ZwAdjustGroupsToken ZwAdjustPrivilegesToken ZwAdjustTokenClaimsAndDeviceGroups ZwAlertResumeThread ZwAlertThread ZwAlertThreadByThreadId ZwAllocateLocallyUniqueId ZwAllocateReserveObject ZwAllocateUserPhysicalPages " \
"ZwAllocateUuids ZwAllocateVirtualMemory ZwAllocateVirtualMemoryEx ZwAlpcAcceptConnectPort ZwAlpcCancelMessage ZwAlpcConnectPort ZwAlpcConnectPortEx ZwAlpcCreatePort ZwAlpcCreatePortSection ZwAlpcCreateResourceReserve ZwAlpcCreateSectionView ZwAlpcCreateSecurityContext ZwAlpcDeletePortSection ZwAlpcDeleteResourceReserve ZwAlpcDeleteSectionView ZwAlpcDeleteSecurityContext ZwAlpcDisconnectPort ZwAlpcImpersonateClientContainerOfPort ZwAlpcImpersonateClientOfPort ZwAlpcOpenSenderProcess ZwAlpcOpenSenderThread ZwAlpcQueryInformation ZwAlpcQueryInformationMessage ZwAlpcRevokeSecurityContext ZwAlpcSendWaitReceivePort ZwAlpcSetInformation ZwApphelpCacheControl ZwAreMappedFilesTheSame ZwAssignProcessToJobObject ZwAssociateWaitCompletionPacket ZwCallEnclave ZwCallbackReturn ZwCancelIoFile ZwCancelIoFileEx ZwCancelSynchronousIoFile ZwCancelTimer ZwCancelTimer2 ZwCancelWaitCompletionPacket ZwClearEvent ZwClose ZwCloseObjectAuditAlarm ZwCommitComplete ZwCommitEnlistment ZwCommitRegistryTransaction ZwCommitTransaction ZwCompactKeys ZwCompareObjects ZwCompareSigningLevels ZwCompareTokens ZwCompleteConnectPort ZwCompressKey ZwConnectPort ZwContinue ZwConvertBetweenAuxiliaryCounterAndPerformanceCounter ZwCreateDebugObject ZwCreateDirectoryObject ZwCreateDirectoryObjectEx ZwCreateEnclave ZwCreateEnlistment ZwCreateEvent ZwCreateEventPair ZwCreateFile ZwCreateIRTimer ZwCreateIoCompletion ZwCreateJobObject ZwCreateJobSet ZwCreateKey ZwCreateKeyTransacted ZwCreateKeyedEvent ZwCreateLowBoxToken ZwCreateMailslotFile ZwCreateMutant ZwCreateNamedPipeFile ZwCreatePagingFile ZwCreatePartition ZwCreatePort ZwCreatePrivateNamespace ZwCreateProcess ZwCreateProcessEx ZwCreateProfile ZwCreateProfileEx ZwCreateRegistryTransaction ZwCreateResourceManager ZwCreateSection ZwCreateSemaphore ZwCreateSymbolicLinkObject ZwCreateThread ZwCreateThreadEx ZwCreateTimer ZwCreateTimer2 ZwCreateToken ZwCreateTokenEx ZwCreateTransaction ZwCreateTransactionManager ZwCreateUserProcess ZwCreateWaitCompletionPacket ZwCreateWaitablePort " \
"ZwCreateWnfStateName ZwCreateWorkerFactory ZwDebugActiveProcess ZwDebugContinue ZwDelayExecution ZwDeleteAtom ZwDeleteBootEntry ZwDeleteDriverEntry ZwDeleteFile ZwDeleteKey ZwDeleteObjectAuditAlarm ZwDeletePrivateNamespace ZwDeleteValueKey ZwDeleteWnfStateData ZwDeleteWnfStateName ZwDeviceIoControlFile ZwDisableLastKnownGood ZwDisplayString ZwDrawText ZwDuplicateObject ZwDuplicateToken ZwEnableLastKnownGood ZwEnumerateBootEntries ZwEnumerateDriverEntries ZwEnumerateKey ZwEnumerateSystemEnvironmentValuesEx ZwEnumerateTransactionObject ZwEnumerateValueKey ZwExtendSection ZwFilterBootOption ZwFilterToken ZwFilterTokenEx ZwFindAtom ZwFlushBuffersFile ZwFlushBuffersFileEx ZwFlushInstallUILanguage ZwFlushInstructionCache ZwFlushKey ZwFlushProcessWriteBuffers ZwFlushVirtualMemory ZwFlushWriteBuffer ZwFreeUserPhysicalPages ZwFreeVirtualMemory ZwFreezeRegistry ZwFreezeTransactions ZwFsControlFile ZwGetCachedSigningLevel ZwGetCompleteWnfStateSubscription ZwGetContextThread ZwGetCurrentProcessorNumber ZwGetCurrentProcessorNumberEx ZwGetDevicePowerState ZwGetMUIRegistryInfo ZwGetNextProcess ZwGetNextThread ZwGetNlsSectionPtr ZwGetNotificationResourceManager ZwGetWriteWatch ZwImpersonateAnonymousToken ZwImpersonateClientOfPort ZwImpersonateThread ZwInitializeEnclave ZwInitializeNlsFiles ZwInitializeRegistry ZwInitiatePowerAction ZwIsProcessInJob ZwIsSystemResumeAutomatic ZwIsUILanguageComitted ZwListenPort ZwLoadDriver ZwLoadEnclaveData ZwLoadHotPatch ZwLoadKey ZwLoadKey2 ZwLoadKeyEx ZwLockFile ZwLockProductActivationKeys ZwLockRegistryKey ZwLockVirtualMemory ZwMakePermanentObject ZwMakeTemporaryObject ZwManagePartition ZwMapCMFModule ZwMapUserPhysicalPages ZwMapUserPhysicalPagesScatter ZwMapViewOfSection ZwMapViewOfSectionEx ZwModifyBootEntry ZwModifyDriverEntry ZwNotifyChangeDirectoryFile ZwNotifyChangeDirectoryFileEx ZwNotifyChangeKey ZwNotifyChangeMultipleKeys ZwNotifyChangeSession ZwOpenDirectoryObject ZwOpenEnlistment ZwOpenEvent ZwOpenEventPair ZwOpenFile ZwOpenIoCompletion " \
"ZwOpenJobObject ZwOpenKey ZwOpenKeyEx ZwOpenKeyTransacted ZwOpenKeyTransactedEx ZwOpenKeyedEvent ZwOpenMutant ZwOpenObjectAuditAlarm ZwOpenPartition ZwOpenPrivateNamespace ZwOpenProcess ZwOpenProcessToken ZwOpenProcessTokenEx ZwOpenRegistryTransaction ZwOpenResourceManager ZwOpenSection ZwOpenSemaphore ZwOpenSession ZwOpenSymbolicLinkObject ZwOpenThread ZwOpenThreadToken ZwOpenThreadTokenEx ZwOpenTimer ZwOpenTransaction ZwOpenTransactionManager ZwPlugPlayControl ZwPowerInformation ZwPrePrepareComplete ZwPrePrepareEnlistment ZwPrepareComplete ZwPrepareEnlistment ZwPrivilegeCheck ZwPrivilegeObjectAuditAlarm ZwPrivilegedServiceAuditAlarm ZwPropagationComplete ZwPropagationFailed ZwProtectVirtualMemory ZwPulseEvent ZwQueryAttributesFile ZwQueryAuxiliaryCounterFrequency ZwQueryBootEntryOrder ZwQueryBootOptions ZwQueryDebugFilterState ZwQueryDefaultLocale ZwQueryDefaultUILanguage ZwQueryDirectoryFile ZwQueryDirectoryFileEx ZwQueryDirectoryObject ZwQueryDriverEntryOrder ZwQueryEaFile ZwQueryEvent ZwQueryFullAttributesFile ZwQueryInformationAtom ZwQueryInformationByName ZwQueryInformationEnlistment ZwQueryInformationFile ZwQueryInformationJobObject ZwQueryInformationPort ZwQueryInformationProcess ZwQueryInformationResourceManager ZwQueryInformationThread ZwQueryInformationToken ZwQueryInformationTransaction ZwQueryInformationTransactionManager ZwQueryInformationWorkerFactory ZwQueryInstallUILanguage ZwQueryIntervalProfile ZwQueryIoCompletion ZwQueryKey ZwQueryLicenseValue ZwQueryMultipleValueKey ZwQueryMutant ZwQueryObject ZwQueryOpenSubKeys ZwQueryOpenSubKeysEx ZwQueryPerformanceCounter ZwQueryPortInformationProcess ZwQueryQuotaInformationFile ZwQuerySection ZwQuerySecurityAttributesToken ZwQuerySecurityObject ZwQuerySecurityPolicy ZwQuerySemaphore ZwQuerySymbolicLinkObject ZwQuerySystemEnvironmentValue ZwQuerySystemEnvironmentValueEx ZwQuerySystemInformation ZwQuerySystemInformationEx ZwQuerySystemTime ZwQueryTimer ZwQueryTimerResolution ZwQueryValueKey ZwQueryVirtualMemory " \
"ZwQueryVolumeInformationFile ZwQueryWnfStateData ZwQueryWnfStateNameInformation ZwQueueApcThread ZwQueueApcThreadEx ZwRaiseException ZwRaiseHardError ZwReadFile ZwReadFileScatter ZwReadOnlyEnlistment ZwReadRequestData ZwReadVirtualMemory ZwRecoverEnlistment ZwRecoverResourceManager ZwRecoverTransactionManager ZwRegisterProtocolAddressInformation ZwRegisterThreadTerminatePort ZwReleaseKeyedEvent ZwReleaseMutant ZwReleaseSemaphore ZwReleaseWorkerFactoryWorker ZwRemoveIoCompletion ZwRemoveIoCompletionEx ZwRemoveProcessDebug ZwRenameKey ZwRenameTransactionManager ZwReplaceKey ZwReplacePartitionUnit ZwReplyPort ZwReplyWaitReceivePort ZwReplyWaitReceivePortEx ZwReplyWaitReplyPort ZwRequestPort ZwRequestWaitReplyPort ZwResetEvent ZwResetWriteWatch ZwRestoreKey ZwResumeProcess ZwResumeThread ZwRevertContainerImpersonation ZwRollbackComplete ZwRollbackEnlistment ZwRollbackRegistryTransaction ZwRollbackTransaction ZwRollforwardTransactionManager ZwSaveKey ZwSaveKeyEx ZwSaveMergedKeys ZwSecureConnectPort ZwSerializeBoot ZwSetBootEntryOrder ZwSetBootOptions ZwSetCachedSigningLevel ZwSetCachedSigningLevel2 ZwSetContextThread ZwSetDebugFilterState ZwSetDefaultHardErrorPort ZwSetDefaultLocale ZwSetDefaultUILanguage ZwSetDriverEntryOrder ZwSetEaFile ZwSetEvent ZwSetEventBoostPriority ZwSetHighEventPair ZwSetHighWaitLowEventPair ZwSetIRTimer ZwSetInformationDebugObject ZwSetInformationEnlistment ZwSetInformationFile ZwSetInformationJobObject ZwSetInformationKey ZwSetInformationObject ZwSetInformationProcess ZwSetInformationResourceManager ZwSetInformationSymbolicLink ZwSetInformationThread ZwSetInformationToken ZwSetInformationTransaction ZwSetInformationTransactionManager ZwSetInformationVirtualMemory ZwSetInformationWorkerFactory ZwSetIntervalProfile ZwSetIoCompletion ZwSetIoCompletionEx ZwSetLdtEntries ZwSetLowEventPair ZwSetLowWaitHighEventPair ZwSetQuotaInformationFile ZwSetSecurityObject ZwSetSystemEnvironmentValue ZwSetSystemEnvironmentValueEx ZwSetSystemInformation ZwSetSystemPowerState " \
"ZwSetSystemTime ZwSetThreadExecutionState ZwSetTimer ZwSetTimer2 ZwSetTimerEx ZwSetTimerResolution ZwSetUuidSeed ZwSetValueKey ZwSetVolumeInformationFile ZwSetWnfProcessNotificationEvent ZwShutdownSystem ZwShutdownWorkerFactory ZwSignalAndWaitForSingleObject ZwSinglePhaseReject ZwStartProfile ZwStopProfile ZwSubscribeWnfStateChange ZwSuspendProcess ZwSuspendThread ZwSystemDebugControl ZwTerminateEnclave ZwTerminateJobObject ZwTerminateProcess ZwTerminateThread ZwTestAlert ZwThawRegistry ZwThawTransactions ZwTraceControl ZwTraceEvent ZwTranslateFilePath ZwUmsThreadYield ZwUnloadDriver ZwUnloadKey ZwUnloadKey2 ZwUnloadKeyEx ZwUnlockFile ZwUnlockVirtualMemory ZwUnmapViewOfSection ZwUnmapViewOfSectionEx ZwUnsubscribeWnfStateChange ZwUpdateWnfStateData ZwVdmControl ZwWaitForAlertByThreadId ZwWaitForDebugEvent ZwWaitForKeyedEvent ZwWaitForMultipleObjects ZwWaitForMultipleObjects32 ZwWaitForSingleObject ZwWaitForWorkViaWorkerFactory ZwWaitHighEventPair ZwWaitLowEventPair ZwWorkerFactoryWorkerReady ZwWriteFile ZwWriteFileGather ZwWriteRequestData ZwWriteVirtualMemory ZwYieldExecution __C_specific_handler __chkstk __isascii __iscsym __iscsymf __misaligned_access __toascii _atoi64 _errno _fltused _i64toa _i64toa_s _i64tow _i64tow_s _itoa _itoa_s _itow _itow_s _lfind _local_unwind _ltoa _ltoa_s _ltow _ltow_s _makepath_s _memccpy _memicmp _setjmp _setjmpex _snprintf _snprintf_s _snscanf_s _snwprintf _snwprintf_s _snwscanf_s _splitpath _splitpath_s _strcmpi _stricmp _strlwr _strlwr_s _strnicmp _strnset_s _strset_s _strupr _strupr_s _swprintf _ui64toa _ui64toa_s _ui64tow _ui64tow_s _ultoa _ultoa_s _ultow _ultow_s _vscprintf _vscwprintf _vsnprintf _vsnprintf_s _vsnwprintf _vsnwprintf_s _vswprintf _wcsicmp _wcslwr _wcslwr_s _wcsnicmp _wcsnset_s _wcsset_s _wcstoi64 _wcstoui64 _wcsupr _wcsupr_s _wmakepath_s _wsplitpath_s _wtoi _wtoi64 _wtol abs atan atan2 atoi atol bsearch bsearch_s ceil cos fabs floor isalnum isalpha iscntrl isdigit isgraph islower isprint ispunct isspace isupper iswalnum " \
"iswalpha iswascii iswctype iswdigit iswgraph iswlower iswprint iswspace iswxdigit isxdigit labs log longjmp mbstowcs memchr memcmp memcpy memcpy_s memmove memmove_s memset pow qsort qsort_s sin sprintf sprintf_s sqrt sscanf sscanf_s strcat strcat_s strchr strcmp strcpy strcpy_s strcspn strlen strncat strncat_s strncmp strncpy strncpy_s strnlen strpbrk strrchr strspn strstr strtok_s strtol strtoul swprintf swprintf_s swscanf_s tan tolower toupper towlower towupper vDbgPrintEx vDbgPrintExWithPrefix vsprintf vsprintf_s vswprintf_s wcscat wcscat_s wcschr wcscmp wcscpy wcscpy_s wcscspn wcslen wcsncat wcsncat_s wcsncmp wcsncpy wcsncpy_s wcsnlen wcspbrk wcsrchr wcsspn wcsstr wcstok_s wcstol wcstombs wcstoul";

char apis_user32[] = "ActivateKeyboardLayout AddClipboardFormatListener AdjustWindowRect AdjustWindowRectEx AdjustWindowRectExForDpi AlignRects AllowForegroundActivation AllowSetForegroundWindow AnimateWindow AnyPopup AppendMenuA AppendMenuW AreDpiAwarenessContextsEqual ArrangeIconicWindows AttachThreadInput BeginDeferWindowPos BeginPaint BlockInput BringWindowToTop BroadcastSystemMessage BroadcastSystemMessageA BroadcastSystemMessageExA BroadcastSystemMessageExW BroadcastSystemMessageW BuildReasonArray CalcMenuBar CalculatePopupWindowPosition CallMsgFilter CallMsgFilterA CallMsgFilterW CallNextHookEx CallWindowProcA CallWindowProcW CancelShutdown CascadeChildWindows CascadeWindows ChangeClipboardChain ChangeDisplaySettingsA ChangeDisplaySettingsExA ChangeDisplaySettingsExW ChangeDisplaySettingsW ChangeMenuA ChangeMenuW ChangeWindowMessageFilter ChangeWindowMessageFilterEx CharLowerA CharLowerBuffA CharLowerBuffW CharLowerW CharNextA CharNextExA CharNextW CharPrevA CharPrevExA CharPrevW CharToOemA CharToOemBuffA CharToOemBuffW CharToOemW CharUpperA CharUpperBuffA CharUpperBuffW CharUpperW CheckDBCSEnabledExt CheckDlgButton CheckMenuItem CheckMenuRadioItem CheckProcessForClipboardAccess CheckProcessSession CheckRadioButton CheckWindowThreadDesktop ChildWindowFromPoint ChildWindowFromPointEx CliImmSetHotKey ClientThreadSetup ClientToScreen ClipCursor CloseClipboard CloseDesktop CloseGestureInfoHandle CloseTouchInputHandle CloseWindow CloseWindowStation ConsoleControl ControlMagnification CopyAcceleratorTableA CopyAcceleratorTableW CopyIcon CopyImage CopyRect CountClipboardFormats CreateAcceleratorTableA CreateAcceleratorTableW CreateCaret CreateCursor CreateDCompositionHwndTarget CreateDesktopA CreateDesktopExA CreateDesktopExW CreateDesktopW CreateDialogIndirectParamA CreateDialogIndirectParamAorW CreateDialogIndirectParamW CreateDialogParamA CreateDialogParamW CreateIcon CreateIconFromResource CreateIconFromResourceEx CreateIconIndirect CreateMDIWindowA CreateMDIWindowW " \
"CreateMenu CreatePalmRejectionDelayZone CreatePopupMenu CreateSystemThreads CreateWindowExA CreateWindowExW CreateWindowInBand CreateWindowInBandEx CreateWindowIndirect CreateWindowStationA CreateWindowStationW CsrBroadcastSystemMessageExW CtxInitUser32 DWMBindCursorToOutputConfig DWMCommitInputSystemOutputConfig DWMSetCursorOrientation DWMSetInputSystemOutputConfig DdeAbandonTransaction DdeAccessData DdeAddData DdeClientTransaction DdeCmpStringHandles DdeConnect DdeConnectList DdeCreateDataHandle DdeCreateStringHandleA DdeCreateStringHandleW DdeDisconnect DdeDisconnectList DdeEnableCallback DdeFreeDataHandle DdeFreeStringHandle DdeGetData DdeGetLastError DdeGetQualityOfService DdeImpersonateClient DdeInitializeA DdeInitializeW DdeKeepStringHandle DdeNameService DdePostAdvise DdeQueryConvInfo DdeQueryNextServer DdeQueryStringA DdeQueryStringW DdeReconnect DdeSetQualityOfService DdeSetUserHandle DdeUnaccessData DdeUninitialize DefFrameProcA DefFrameProcW DefMDIChildProcA DefMDIChildProcW DefRawInputProc DeferWindowPos DeferWindowPosAndBand DelegateInput DeleteMenu DeregisterShellHookWindow DestroyAcceleratorTable DestroyCaret DestroyCursor DestroyDCompositionHwndTarget DestroyIcon DestroyMenu DestroyPalmRejectionDelayZone DestroyReasons DestroyWindow DialogBoxIndirectParamA DialogBoxIndirectParamAorW DialogBoxIndirectParamW DialogBoxParamA DialogBoxParamW DisableProcessWindowsGhosting DispatchMessageA DispatchMessageW DisplayConfigGetDeviceInfo DisplayConfigSetDeviceInfo DisplayExitWindowsWarnings DlgDirListA DlgDirListComboBoxA DlgDirListComboBoxW DlgDirListW DlgDirSelectComboBoxExA DlgDirSelectComboBoxExW DlgDirSelectExA DlgDirSelectExW DoSoundConnect DoSoundDisconnect DragDetect DragObject DrawAnimatedRects DrawCaption DrawCaptionTempA DrawCaptionTempW DrawEdge DrawFocusRect DrawFrame DrawFrameControl DrawIcon DrawIconEx DrawMenuBar DrawMenuBarTemp DrawStateA DrawStateW DrawTextA DrawTextExA DrawTextExW DrawTextW " \
"DwmGetDxRgn DwmGetDxSharedSurface DwmGetRemoteSessionOcclusionEvent DwmGetRemoteSessionOcclusionState DwmKernelShutdown DwmKernelStartup DwmLockScreenUpdates DwmValidateWindow EditWndProc EmptyClipboard EnableMenuItem EnableMouseInPointer EnableNonClientDpiScaling EnableOneCoreTransformMode EnableScrollBar EnableSessionForMMCSS EnableWindow EndDeferWindowPos EndDeferWindowPosEx EndDialog EndMenu EndPaint EndTask EnterReaderModeHelper EnumChildWindows EnumClipboardFormats EnumDesktopWindows EnumDesktopsA EnumDesktopsW EnumDisplayDevicesA EnumDisplayDevicesW EnumDisplayMonitors EnumDisplaySettingsA EnumDisplaySettingsExA EnumDisplaySettingsExW EnumDisplaySettingsW EnumPropsA EnumPropsExA EnumPropsExW EnumPropsW EnumThreadWindows EnumWindowStationsA EnumWindowStationsW EnumWindows EqualRect EvaluateProximityToPolygon EvaluateProximityToRect ExcludeUpdateRgn ExitWindowsEx FillRect FindWindowA FindWindowExA FindWindowExW FindWindowW FlashWindow FlashWindowEx FrameRect FreeDDElParam FrostCrashedWindow GetActiveWindow GetAltTabInfo GetAltTabInfoA GetAltTabInfoW GetAncestor GetAppCompatFlags GetAppCompatFlags2 GetAsyncKeyState GetAutoRotationState GetAwarenessFromDpiAwarenessContext GetCIMSSM GetCapture GetCaretBlinkTime GetCaretPos GetClassInfoA GetClassInfoExA GetClassInfoExW GetClassInfoW GetClassLongA GetClassLongPtrA GetClassLongPtrW GetClassLongW GetClassNameA GetClassNameW GetClassWord GetClientRect GetClipCursor GetClipboardAccessToken GetClipboardData GetClipboardFormatNameA GetClipboardFormatNameW GetClipboardOwner GetClipboardSequenceNumber GetClipboardViewer GetComboBoxInfo GetCurrentInputMessageSource GetCursor GetCursorFrameInfo GetCursorInfo GetCursorPos GetDC GetDCEx GetDesktopID GetDesktopWindow GetDialogBaseUnits GetDialogControlDpiChangeBehavior GetDialogDpiChangeBehavior GetDisplayAutoRotationPreferences GetDisplayConfigBufferSizes GetDlgCtrlID GetDlgItem GetDlgItemInt GetDlgItemTextA GetDlgItemTextW GetDoubleClickTime GetDpiForMonitorInternal GetDpiForSystem " \
"GetDpiForWindow GetDpiFromDpiAwarenessContext GetFocus GetForegroundWindow GetGUIThreadInfo GetGestureConfig GetGestureExtraArgs GetGestureInfo GetGuiResources GetIconInfo GetIconInfoExA GetIconInfoExW GetInputDesktop GetInputLocaleInfo GetInputState GetInternalWindowPos GetKBCodePage GetKeyNameTextA GetKeyNameTextW GetKeyState GetKeyboardLayout GetKeyboardLayoutList GetKeyboardLayoutNameA GetKeyboardLayoutNameW GetKeyboardState GetKeyboardType GetLastActivePopup GetLastInputInfo GetLayeredWindowAttributes GetListBoxInfo GetMagnificationDesktopColorEffect GetMagnificationDesktopMagnification GetMagnificationDesktopSamplingMode GetMagnificationLensCtxInformation GetMenu GetMenuBarInfo GetMenuCheckMarkDimensions GetMenuContextHelpId GetMenuDefaultItem GetMenuInfo GetMenuItemCount GetMenuItemID GetMenuItemInfoA GetMenuItemInfoW GetMenuItemRect GetMenuState GetMenuStringA GetMenuStringW GetMessageA GetMessageExtraInfo GetMessagePos GetMessageTime GetMessageW GetMonitorInfoA GetMonitorInfoW GetMouseMovePointsEx GetNextDlgGroupItem GetNextDlgTabItem GetOpenClipboardWindow GetParent GetPhysicalCursorPos GetPointerCursorId GetPointerDevice GetPointerDeviceCursors GetPointerDeviceProperties GetPointerDeviceRects GetPointerDevices GetPointerFrameArrivalTimes GetPointerFrameInfo GetPointerFrameInfoHistory GetPointerFramePenInfo GetPointerFramePenInfoHistory GetPointerFrameTouchInfo GetPointerFrameTouchInfoHistory GetPointerInfo GetPointerInfoHistory GetPointerInputTransform GetPointerPenInfo GetPointerPenInfoHistory GetPointerTouchInfo GetPointerTouchInfoHistory GetPointerType GetPriorityClipboardFormat GetProcessDefaultLayout GetProcessDpiAwarenessInternal GetProcessUIContextInformation GetProcessWindowStation GetProgmanWindow GetPropA GetPropW GetQueueStatus GetRawInputBuffer GetRawInputData GetRawInputDeviceInfoA GetRawInputDeviceInfoW GetRawInputDeviceList GetRawPointerDeviceData GetReasonTitleFromReasonCode GetRegisteredRawInputDevices GetScrollBarInfo GetScrollInfo GetScrollPos " \
"GetScrollRange GetSendMessageReceiver GetShellWindow GetSubMenu GetSysColor GetSysColorBrush GetSystemDpiForProcess GetSystemMenu GetSystemMetrics GetSystemMetricsForDpi GetTabbedTextExtentA GetTabbedTextExtentW GetTaskmanWindow GetThreadDesktop GetThreadDpiAwarenessContext GetThreadDpiHostingBehavior GetTitleBarInfo GetTopLevelWindow GetTopWindow GetTouchInputInfo GetUnpredictedMessagePos GetUpdateRect GetUpdateRgn GetUpdatedClipboardFormats GetUserObjectInformationA GetUserObjectInformationW GetUserObjectSecurity GetWinStationInfo GetWindow GetWindowBand GetWindowCompositionAttribute GetWindowCompositionInfo GetWindowContextHelpId GetWindowDC GetWindowDisplayAffinity GetWindowDpiAwarenessContext GetWindowDpiHostingBehavior GetWindowFeedbackSetting GetWindowInfo GetWindowLongA GetWindowLongPtrA GetWindowLongPtrW GetWindowLongW GetWindowMinimizeRect GetWindowModuleFileName GetWindowModuleFileNameA GetWindowModuleFileNameW GetWindowPlacement GetWindowProcessHandle GetWindowRect GetWindowRgn GetWindowRgnBox GetWindowRgnEx GetWindowTextA GetWindowTextLengthA GetWindowTextLengthW GetWindowTextW GetWindowThreadProcessId GetWindowWord GhostWindowFromHungWindow GrayStringA GrayStringW HandleDelegatedInput HideCaret HiliteMenuItem HungWindowFromGhostWindow IMPGetIMEA IMPGetIMEW IMPQueryIMEA IMPQueryIMEW IMPSetIMEA IMPSetIMEW ImpersonateDdeClientWindow InSendMessage InSendMessageEx InflateRect InheritWindowMonitor InitDManipHook InitializeGenericHidInjection InitializeInputDeviceInjection InitializeLpkHooks InitializePointerDeviceInjection InitializePointerDeviceInjectionEx InitializeTouchInjection InjectDeviceInput InjectGenericHidInput InjectKeyboardInput InjectMouseInput InjectPointerInput InjectTouchInput InsertMenuA InsertMenuItemA InsertMenuItemW InsertMenuW InternalGetWindowIcon InternalGetWindowText IntersectRect InvalidateRect InvalidateRgn InvertRect IsCharAlphaA IsCharAlphaNumericA IsCharAlphaNumericW IsCharAlphaW IsCharLowerA IsCharLowerW IsCharUpperA IsCharUpperW " \
"IsChild IsClipboardFormatAvailable IsDialogMessage IsDialogMessageA IsDialogMessageW IsDlgButtonChecked IsGUIThread IsHungAppWindow IsIconic IsImmersiveProcess IsInDesktopWindowBand IsMenu IsMouseInPointerEnabled IsOneCoreTransformMode IsProcessDPIAware IsQueueAttached IsRectEmpty IsSETEnabled IsServerSideWindow IsThreadDesktopComposited IsThreadMessageQueueAttached IsThreadTSFEventAware IsTopLevelWindow IsTouchWindow IsValidDpiAwarenessContext IsWinEventHookInstalled IsWindow IsWindowArranged IsWindowEnabled IsWindowInDestroy IsWindowRedirectedForPrint IsWindowUnicode IsWindowVisible IsWow64Message IsZoomed KillTimer LoadAcceleratorsA LoadAcceleratorsW LoadBitmapA LoadBitmapW LoadCursorA LoadCursorFromFileA LoadCursorFromFileW LoadCursorW LoadIconA LoadIconW LoadImageA LoadImageW LoadKeyboardLayoutA LoadKeyboardLayoutEx LoadKeyboardLayoutW LoadLocalFonts LoadMenuA LoadMenuIndirectA LoadMenuIndirectW LoadMenuW LoadRemoteFonts LoadStringA LoadStringW LockSetForegroundWindow LockWindowStation LockWindowUpdate LockWorkStation LogicalToPhysicalPoint LogicalToPhysicalPointForPerMonitorDPI LookupIconIdFromDirectory LookupIconIdFromDirectoryEx MBToWCSEx MBToWCSExt MB_GetString MITActivateInputProcessing MITBindInputTypeToMonitors MITCoreMsgKGetConnectionHandle MITCoreMsgKOpenConnectionTo MITCoreMsgKSend MITDeactivateInputProcessing MITDisableMouseIntercept MITDispatchCompletion MITEnableMouseIntercept MITGetCursorUpdateHandle MITInjectLegacyISMTouchFrame MITRegisterManipulationThread MITSetForegroundRoutingInfo MITSetInputCallbacks MITSetInputDelegationMode MITSetLastInputRecipient MITSetManipulationInputTarget MITStopAndEndInertia MITSynthesizeMouseInput MITSynthesizeMouseWheel MITSynthesizeTouchInput MITUpdateInputGlobals MITWaitForMultipleObjectsEx MakeThreadTSFEventAware MapDialogRect MapVirtualKeyA MapVirtualKeyExA MapVirtualKeyExW MapVirtualKeyW MapVisualRelativePoints MapWindowPoints MenuItemFromPoint MenuWindowProcA MenuWindowProcW MessageBeep MessageBoxA MessageBoxExA " \
"MessageBoxExW MessageBoxIndirectA MessageBoxIndirectW MessageBoxTimeoutA MessageBoxTimeoutW MessageBoxW ModifyMenuA ModifyMenuW MonitorFromPoint MonitorFromRect MonitorFromWindow MoveWindow MsgWaitForMultipleObjects MsgWaitForMultipleObjectsEx NotifyOverlayWindow NotifyWinEvent OemKeyScan OemToCharA OemToCharBuffA OemToCharBuffW OemToCharW OffsetRect OpenClipboard OpenDesktopA OpenDesktopW OpenIcon OpenInputDesktop OpenThreadDesktop OpenWindowStationA OpenWindowStationW PackDDElParam PackTouchHitTestingProximityEvaluation PaintDesktop PaintMenuBar PaintMonitor PeekMessageA PeekMessageW PhysicalToLogicalPoint PhysicalToLogicalPointForPerMonitorDPI PostMessageA PostMessageW PostQuitMessage PostThreadMessageA PostThreadMessageW PrintWindow PrivateExtractIconExA PrivateExtractIconExW PrivateExtractIconsA PrivateExtractIconsW PrivateRegisterICSProc PtInRect QueryBSDRWindow QueryDisplayConfig QuerySendMessage RIMAddInputObserver RIMAreSiblingDevices RIMDeviceIoControl RIMEnableMonitorMappingForDevice RIMFreeInputBuffer RIMGetDevicePreparsedData RIMGetDevicePreparsedDataLockfree RIMGetDeviceProperties RIMGetDevicePropertiesLockfree RIMGetPhysicalDeviceRect RIMGetSourceProcessId RIMObserveNextInput RIMOnPnpNotification RIMOnTimerNotification RIMReadInput RIMRegisterForInput RIMRemoveInputObserver RIMSetTestModeStatus RIMUnregisterForInput RIMUpdateInputObserverRegistration RealChildWindowFromPoint RealGetWindowClass RealGetWindowClassA RealGetWindowClassW ReasonCodeNeedsBugID ReasonCodeNeedsComment RecordShutdownReason RedrawWindow RegisterBSDRWindow RegisterClassA RegisterClassExA RegisterClassExW RegisterClassW RegisterClipboardFormatA RegisterClipboardFormatW RegisterDManipHook RegisterDeviceNotificationA RegisterDeviceNotificationW RegisterErrorReportingDialog RegisterFrostWindow RegisterGhostWindow RegisterHotKey RegisterLogonProcess RegisterMessagePumpHook RegisterPointerDeviceNotifications RegisterPointerInputTarget RegisterPointerInputTargetEx RegisterPowerSettingNotification " \
"RegisterRawInputDevices RegisterServicesProcess RegisterSessionPort RegisterShellHookWindow RegisterSuspendResumeNotification RegisterSystemThread RegisterTasklist RegisterTouchHitTestingWindow RegisterTouchWindow RegisterUserApiHook RegisterWindowMessageA RegisterWindowMessageW ReleaseCapture ReleaseDC ReleaseDwmHitTestWaiters RemoveClipboardFormatListener RemoveInjectionDevice RemoveMenu RemovePropA RemovePropW RemoveThreadTSFEventAwareness ReplyMessage ReportInertia ResolveDesktopForWOW ReuseDDElParam ScreenToClient ScrollChildren ScrollDC ScrollWindow ScrollWindowEx SendDlgItemMessageA SendDlgItemMessageW SendIMEMessageExA SendIMEMessageExW SendInput SendMessageA SendMessageCallbackA SendMessageCallbackW SendMessageTimeoutA SendMessageTimeoutW SendMessageW SendNotifyMessageA SendNotifyMessageW SetActiveWindow SetCapture SetCaretBlinkTime SetCaretPos SetClassLongA SetClassLongPtrA SetClassLongPtrW SetClassLongW SetClassWord SetClipboardData SetClipboardViewer SetCoalescableTimer SetCoreWindow SetCursor SetCursorContents SetCursorPos SetDebugErrorLevel SetDeskWallpaper SetDesktopColorTransform SetDialogControlDpiChangeBehavior SetDialogDpiChangeBehavior SetDisplayAutoRotationPreferences SetDisplayConfig SetDlgItemInt SetDlgItemTextA SetDlgItemTextW SetDoubleClickTime SetFeatureReportResponse SetFocus SetForegroundWindow SetGestureConfig SetInternalWindowPos SetKeyboardState SetLastErrorEx SetLayeredWindowAttributes SetMagnificationDesktopColorEffect SetMagnificationDesktopMagnification SetMagnificationDesktopSamplingMode SetMagnificationLensCtxInformation SetMenu SetMenuContextHelpId SetMenuDefaultItem SetMenuInfo SetMenuItemBitmaps SetMenuItemInfoA SetMenuItemInfoW SetMessageExtraInfo SetMessageQueue SetMirrorRendering SetParent SetPhysicalCursorPos SetProcessDPIAware SetProcessDefaultLayout SetProcessDpiAwarenessContext SetProcessDpiAwarenessInternal SetProcessRestrictionExemption SetProcessWindowStation SetProgmanWindow SetPropA SetPropW SetRect SetRectEmpty SetScrollInfo " \
"SetScrollPos SetScrollRange SetShellWindow SetShellWindowEx SetSysColors SetSysColorsTemp SetSystemCursor SetSystemMenu SetTaskmanWindow SetThreadDesktop SetThreadDpiAwarenessContext SetThreadDpiHostingBehavior SetThreadInputBlocked SetTimer SetUserObjectInformationA SetUserObjectInformationW SetUserObjectSecurity SetWinEventHook SetWindowBand SetWindowCompositionAttribute SetWindowCompositionTransition SetWindowContextHelpId SetWindowDisplayAffinity SetWindowFeedbackSetting SetWindowLongA SetWindowLongPtrA SetWindowLongPtrW SetWindowLongW SetWindowPlacement SetWindowPos SetWindowRgn SetWindowRgnEx SetWindowStationUser SetWindowTextA SetWindowTextW SetWindowWord SetWindowsHookA SetWindowsHookExA SetWindowsHookExW SetWindowsHookW ShowCaret ShowCursor ShowOwnedPopups ShowScrollBar ShowStartGlass ShowSystemCursor ShowWindow ShowWindowAsync ShutdownBlockReasonCreate ShutdownBlockReasonDestroy ShutdownBlockReasonQuery SignalRedirectionStartComplete SkipPointerFrameMessages SoftModalMessageBox SoundSentry SubtractRect SwapMouseButton SwitchDesktop SwitchDesktopWithFade SwitchToThisWindow SystemParametersInfoA SystemParametersInfoForDpi SystemParametersInfoW TabbedTextOutA TabbedTextOutW TileChildWindows TileWindows ToAscii ToAsciiEx ToUnicode ToUnicodeEx TrackMouseEvent TrackPopupMenu TrackPopupMenuEx TranslateAccelerator TranslateAcceleratorA TranslateAcceleratorW TranslateMDISysAccel TranslateMessage TranslateMessageEx UndelegateInput UnhookWinEvent UnhookWindowsHook UnhookWindowsHookEx UnionRect UnloadKeyboardLayout UnlockWindowStation UnpackDDElParam UnregisterClassA UnregisterClassW UnregisterDeviceNotification UnregisterHotKey UnregisterMessagePumpHook UnregisterPointerInputTarget UnregisterPointerInputTargetEx UnregisterPowerSettingNotification UnregisterSessionPort UnregisterSuspendResumeNotification UnregisterTouchWindow UnregisterUserApiHook UpdateDefaultDesktopThumbnail UpdateLayeredWindow UpdateLayeredWindowIndirect UpdatePerUserSystemParameters UpdateWindow UpdateWindowInputSinkHints " \
"User32InitializeImmEntryTable UserClientDllInitialize UserHandleGrantAccess UserLpkPSMTextOut UserLpkTabbedTextOut UserRealizePalette UserRegisterWowHandlers VRipOutput VTagOutput ValidateRect ValidateRgn VkKeyScanA VkKeyScanExA VkKeyScanExW VkKeyScanW WCSToMBEx WINNLSEnableIME WINNLSGetEnableStatus WINNLSGetIMEHotkey WaitForInputIdle WaitForRedirectionStartComplete WaitMessage WinHelpA WinHelpW WindowFromDC WindowFromPhysicalPoint WindowFromPoint _UserTestTokenForInteractive gSharedInfo gapfnScSendMessage keybd_event mouse_event wsprintfA wsprintfW wvsprintfA wvsprintfW";

bool ModuleBoundsHookCheckSingle(HMODULE dll, char* apiList)
{
    MODULEINFO moduleInfo;
    if (GetModuleInformation(GetCurrentProcess(), dll, &moduleInfo, sizeof(MODULEINFO)) == FALSE)
    {
        // todo: error condition
        return FALSE;
    }

    PVOID moduleBottom = moduleInfo.lpBaseOfDll;
    PVOID moduleTop = reinterpret_cast<PVOID>((reinterpret_cast<unsigned char*>(moduleBottom) + moduleInfo.SizeOfImage));

    char* currentAPI = NULL;
    char* nextAPI = NULL;
    currentAPI = strtok_s(apiList, " ", &nextAPI);

    bool foundHook = false;

    while (currentAPI != NULL)
    {
        PVOID procAddr = GetProcAddress(dll, currentAPI);
        if (procAddr != NULL)
        {
            if (procAddr < moduleBottom || procAddr >= moduleTop)
            {
                foundHook = true;
                //printf("Caught hook on API '%s'\n", currentAPI);
            }
        }

        currentAPI = strtok_s(NULL, " ", &nextAPI);
    }

    return foundHook;
}

BOOL ModuleBoundsHookCheck()
{
    bool foundHook = ModuleBoundsHookCheckSingle(LoadLibrary((LPCWSTR)PRINT_HIDE_STR("kernel32.dll")), apis_kernel32) &&
        ModuleBoundsHookCheckSingle(LoadLibrary((LPCWSTR)PRINT_HIDE_STR("ntdll.dll")), apis_ntdll) &&
        ModuleBoundsHookCheckSingle(LoadLibrary((LPCWSTR)PRINT_HIDE_STR("user32.dll")), apis_user32);

    return foundHook ? TRUE : FALSE;
}

BOOL MemoryBreakpoints_PageGuard()
{
    UCHAR* pMem = NULL;
    SYSTEM_INFO SystemInfo = { 0 };
    DWORD OldProtect = 0;
    PVOID pAllocation = NULL; // Get the page size for the system 

    // Retrieves information about the current system.
    hash_GetSystemInfo(&SystemInfo);

    // Allocate memory 
    pAllocation = hash_VirtualAlloc(NULL, SystemInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (pAllocation == NULL)
        return FALSE;

    // Write a ret to the buffer (opcode 0xc3)
    RtlFillMemory(pAllocation, 1, 0xC3);

    // Make the page a guard page         
    if (hash_VirtualProtect(pAllocation, SystemInfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect) == 0)
        return FALSE;

    __try
    {
        ((void(*)())pAllocation)(); // Exception or execution, which shall it be :D?
    }
    __except (GetExceptionCode() == STATUS_GUARD_PAGE_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        hash_VirtualFree(pAllocation, 0, MEM_RELEASE);
        return FALSE;
    }

    hash_VirtualFree(pAllocation, 0, MEM_RELEASE);
    return TRUE;
}

static BOOL SwallowedException2 = TRUE;

static LONG CALLBACK VectoredHandler2(
    _In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
    SwallowedException2 = FALSE;
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
    {
        //Increase EIP/RIP to continue execution.
#ifdef _WIN64
        ExceptionInfo->ContextRecord->Rip++;
#else
        ExceptionInfo->ContextRecord->Eip++;
#endif
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}


VOID ErasePEHeaderFromMemory()
{
    //_tprintf(_T("[*] Erasing PE header from memory\n"));
    DWORD OldProtect = 0;

    // Get base address of module
    char* pBaseAddr = (char*)GetModuleHandle(NULL);

    // Change memory protection
    hash_VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &OldProtect);

    // Erase the header
    SecureZeroMemory(pBaseAddr, 4096);
}





BOOL HardwareBreakpoints()
{
    BOOL bResult = FALSE;

    // This structure is key to the function and is the 
    // medium for detection and removal
    PCONTEXT ctx = PCONTEXT(hash_VirtualAlloc(NULL, sizeof(CONTEXT), MEM_COMMIT, PAGE_READWRITE));

    if (ctx) {

        SecureZeroMemory(ctx, sizeof(CONTEXT));

        // The CONTEXT structure is an in/out parameter therefore we have
        // to set the flags so Get/SetThreadContext knows what to set or get.
        ctx->ContextFlags = CONTEXT_DEBUG_REGISTERS;

        // Get the registers
        if (hash_GetThreadContext(hash_GetCurrentThread(), ctx)) {

            // Now we can check for hardware breakpoints, its not 
            // necessary to check Dr6 and Dr7, however feel free to
            if (ctx->Dr0 != 0 || ctx->Dr1 != 0 || ctx->Dr2 != 0 || ctx->Dr3 != 0)
                bResult = TRUE;
        }

        hash_VirtualFree(ctx, 0, MEM_RELEASE);
        }

    return bResult;
    }

VOID My_Critical_Function()
{
    int a = 1;
    int b = 2;
    int c = a + b;
    //_tprintf(_T("I am critical function, you should protect against int3 bps %d"), c);
}


VOID Myfunction_Adresss_Next()
{
    My_Critical_Function();
};

BOOL SoftwareBreakpoints()
{
    //NOTE this check might not work on x64 because of alignment 0xCC bytes
    size_t sSizeToCheck = (size_t)(Myfunction_Adresss_Next)-(size_t)(My_Critical_Function);
    PUCHAR Critical_Function = (PUCHAR)My_Critical_Function;

    for (size_t i = 0; i < sSizeToCheck; i++) {
        if (Critical_Function[i] == 0xCC) // Adding another level of indirection : 0xCC xor 0x55 = 0x99
            return TRUE;
    }
    return FALSE;
}

__forceinline BOOL process_job()
{
  auto found_problem = FALSE;
  const DWORD job_process_struct_size = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + sizeof(ULONG_PTR) * 1024;
  auto job_process_id_list = static_cast<JOBOBJECT_BASIC_PROCESS_ID_LIST *>(malloc(
                               job_process_struct_size));
  if (job_process_id_list)
  {
    SecureZeroMemory(job_process_id_list, job_process_struct_size);
    job_process_id_list->NumberOfProcessIdsInList = 1024;
    if (hash_QueryInformationJobObject(nullptr, JobObjectBasicProcessIdList, job_process_id_list, job_process_struct_size,
                                       nullptr))
    {
      auto ok_processes = 0;
      for (DWORD i = 0; i < job_process_id_list->NumberOfAssignedProcesses; i++)
      {
        const auto process_id = job_process_id_list->ProcessIdList[i];
        if (process_id == static_cast<ULONG_PTR>(hash_GetCurrentProcessId()))
        {
          ok_processes++;
        }
        else
        {
          const auto h_job_process = hash_OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, static_cast<DWORD>(process_id));
          if (h_job_process != nullptr)
          {
            const auto process_name_buffer_size = 4096;
            const auto process_name = static_cast<LPWSTR>(malloc(sizeof(TCHAR) * process_name_buffer_size));
            if (process_name)
            {
              RtlSecureZeroMemory(process_name, sizeof(TCHAR) * process_name_buffer_size);
              if (hash_K32GetProcessImageFileNameW(h_job_process, process_name, process_name_buffer_size) > 0)
              {
                std::wstring pnStr(process_name);
                if (pnStr.find(static_cast<std::wstring>(GetWC_l((LPCSTR)PRINT_HIDE_STR("\\Windows\\System32\\conhost.exe")))) != std::string::npos)
                {
                  ok_processes++;
                }
              }
              free(process_name);
            }
            hash_CloseHandle(h_job_process);
          }
        }
      }
      found_problem = ok_processes != static_cast<int>(job_process_id_list->NumberOfAssignedProcesses);
    }
    free(job_process_id_list);
  }
  return found_problem;
}

void set_handle_informatiom_protected_handle_helper()
{
  const auto h_mutex = hash_CreateMutexW(nullptr, FALSE, GetWC_l((LPCSTR)PRINT_HIDE_STR("923482934823948")));
  hash_SetHandleInformation(h_mutex, HANDLE_FLAG_PROTECT_FROM_CLOSE, HANDLE_FLAG_PROTECT_FROM_CLOSE);
  hash_CloseHandle(h_mutex);
}

BOOL set_handle_informatiom_protected_handle()
{
  __try
  {
    set_handle_informatiom_protected_handle_helper();
  }
  __except (EXCEPTION_EXECUTE_HANDLER)
  {
    return TRUE;
  }
  return FALSE;
}

__forceinline BOOL titan_hide_check()
{
  const auto ntdll = hash_GetModuleHandleW(NTDLL_);
  const auto nt_query_system_information = reinterpret_cast<t_nt_query_system_information>(hash_GetProcAddress(
        ntdll, (LPCSTR)PRINT_HIDE_STR("NtQuerySystemInformation")));
  SYSTEM_CODEINTEGRITY_INFORMATION c_info;
  c_info.Length = sizeof c_info;
  nt_query_system_information(SystemCodeIntegrityInformation, &c_info, sizeof c_info, nullptr);
  const int ret = c_info.CodeIntegrityOptions & CODEINTEGRITY_OPTION_TESTSIGN || c_info.CodeIntegrityOptions &
                  CODEINTEGRITY_OPTION_DEBUGMODE_ENABLED;
  return ret;
}

__forceinline BOOL NtQuerySystemInformation_SystemKernelDebuggerInformation()
{
  const int SystemKernelDebuggerInformation = 0x23;
  SYSTEM_KERNEL_DEBUGGER_INFORMATION KdDebuggerInfo;
  const auto ntdll = hash_GetModuleHandleW(NTDLL_);
  const auto NtQuerySystemInformation = reinterpret_cast<t_nt_query_system_information>(hash_GetProcAddress(
                                          ntdll, (LPCSTR)PRINT_HIDE_STR("NtQuerySystemInformation")));
  NTSTATUS Status = NtQuerySystemInformation(SystemKernelDebuggerInformation, &KdDebuggerInfo, sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION), NULL);
  if (Status >= 0)
  {
    if (KdDebuggerInfo.KernelDebuggerEnabled || !KdDebuggerInfo.KernelDebuggerNotPresent)
      return TRUE;
  }
  return FALSE;
}

__forceinline BOOL SharedUserData_KernelDebugger()
{
  const ULONG_PTR UserSharedData = 0x7FFE0000;
  const UCHAR KdDebuggerEnabledByte = *(UCHAR *)(UserSharedData + 0x2D4);
  const BOOLEAN KdDebuggerEnabled = (KdDebuggerEnabledByte & 0x1) == 0x1;
  const BOOLEAN KdDebuggerNotPresent = (KdDebuggerEnabledByte & 0x2) == 0;
  if (KdDebuggerEnabled || !KdDebuggerNotPresent)
    return TRUE;
  return FALSE;
}