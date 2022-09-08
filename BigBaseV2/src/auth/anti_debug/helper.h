#pragma once
#include <auth/anti_debug/winapi_hash/hash_work.hpp>
#include <DbgHelp.h>

typedef struct {
    LPVOID lpVirtualAddress;
    DWORD dwSizeOfRawData;
} SECTIONINFO, * PSECTIONINFO;

typedef struct {
    DWORD64 dwRealHash;
    SECTIONINFO SectionInfo;
} HASHSET, * PHASHSET;

std::string gen_random(const int len) {

    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand((unsigned)time(NULL));

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}

BOOL Switch()
{
    auto name = gen_random(50);
    HDESK hNewDesktop = hash_CreateDesktopA(
        name.c_str(),
        NULL,
        NULL,
        0,
        DESKTOP_CREATEWINDOW | DESKTOP_WRITEOBJECTS | DESKTOP_SWITCHDESKTOP,
        NULL);
    if (!hNewDesktop)
        ExitProcess(420);

    return hash_SwitchDesktop(hNewDesktop);
}
// the bluescreen below
inline void oopsiewoopsie() {
    LOG(INFO) << "dEBUGGER dETECTED";
    //BOOLEAN bl;
    //unsigned long response;
    //hash_RtlAdjustPrivilege(19, true, false, &bl);
    //hash_NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
    //////(bool*)0 = true; //Overflow int, results in a crash.
    //Switch();
    //ExitProcess(69);

}

int GetAllModule(std::vector<LPVOID>& modules) {
    MODULEENTRY32W mEntry;
    memset(&mEntry, 0, sizeof(mEntry));
    mEntry.dwSize = sizeof(MODULEENTRY32);

    DWORD curPid = GetCurrentProcessId();

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, NULL);
    if (Module32FirstW(hSnapshot, &mEntry)) {
        do {
            modules.emplace_back(mEntry.modBaseAddr);
        } while (Module32NextW(hSnapshot, &mEntry));
    }

    CloseHandle(hSnapshot);

    if (modules.empty()) {
        return -1;
    }

    return 0;
}

int GetTextSectionInfo(LPVOID lpModBaseAddr, PSECTIONINFO info) {
    PIMAGE_NT_HEADERS pNtHdr = ImageNtHeader(lpModBaseAddr);
    PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(pNtHdr + 1);

    LPVOID lpTextAddr = NULL;
    DWORD dwSizeOfRawData = NULL;

    for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; ++i) {
        char* name = (char*)pSectionHeader->Name;

        if (!strcmp(name, ".text")) {
            info->lpVirtualAddress = (LPVOID)((DWORD64)lpModBaseAddr + pSectionHeader->VirtualAddress);
            info->dwSizeOfRawData = pSectionHeader->SizeOfRawData;
            break;
        }

        ++pSectionHeader;
    }

    if (info->dwSizeOfRawData == NULL) {
        return -1;
    }

    return 0;
}

DWORD64 HashSection(LPVOID lpSectionAddress, DWORD dwSizeOfRawData) {
    DWORD64 hash = 0;
    BYTE* str = (BYTE*)lpSectionAddress;
    for (int i = 0; i < dwSizeOfRawData; ++i, ++str) {
        if (*str) {
            hash = *str + (hash << 6) + (hash << 16) - hash;
        }
    }

    return hash;
}

bool bTerminateThread = false;

void CheckTextHash(PHASHSET pHashSet) {
    DWORD64 dwRealHash = pHashSet->dwRealHash;
    DWORD dwSizeOfRawData = pHashSet->SectionInfo.dwSizeOfRawData;
    LPVOID lpVirtualAddress = pHashSet->SectionInfo.lpVirtualAddress;

    while (1) {
        Sleep(1000);

        DWORD64 dwCurrentHash = HashSection(lpVirtualAddress, dwSizeOfRawData);
        if (dwRealHash != dwCurrentHash) {
            oopsiewoopsie();

        }

        if (bTerminateThread) {
            return;
        }
    }
}

int ExitThreads(std::vector<std::thread>& threads) {
    bTerminateThread = true;

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}

