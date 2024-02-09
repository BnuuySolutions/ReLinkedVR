#include <windows.h>
#include <MinHook.h>
#include "offsets.h"
#include "util.h"

using namespace oculus_ameliorated;

// Disables file certificate validation inside OVRService.
int64_t (*OVRUtil__ValidateFile)(LPCWSTR, HANDLE*) = nullptr;
int64_t OVRUtil__ValidateFileHook(LPCWSTR fileName, HANDLE** fileHandle) {
  if (!fileHandle || !fileName) {
    return 2; // ??
  }

  HANDLE hFile = CreateFileW(fileName, 0x80000000, 1, 0, 3, 1, 0);
  if (hFile == INVALID_HANDLE_VALUE) {
    return 3; // ??
  }

  fileHandle[0] = &hFile;
  return 0; // NoError(?)
}

void WINAPI Main() {
  util::InitProxy();

  if (MH_Initialize() != MH_STATUS::MH_OK) {
    MessageBoxA(NULL, "Failed to initialize MinHook!", "OVRServiceHook (Oculus Ameliorated)", MB_ICONERROR | MB_OK);
  }

  auto OVRUtil__ValidateFileAddress = reinterpret_cast<PBYTE>(util::GetBaseAddress() + offsets::OVRUtil::ValidateFile);

  MH_CreateHook(OVRUtil__ValidateFileAddress, OVRUtil__ValidateFileHook, reinterpret_cast<LPVOID*>(&OVRUtil__ValidateFile));
  MH_EnableHook(OVRUtil__ValidateFileAddress);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  if (dwReason == DLL_PROCESS_ATTACH) {
    Main();
  }
  return TRUE;
}

