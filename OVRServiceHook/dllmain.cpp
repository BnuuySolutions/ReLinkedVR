#include <windows.h>
#include <MinHook.h>
#include "proxy.h"
#include "util.h"

// Disables library certificate validation inside OVRService.
int64_t VerifyLibraryHook(LPCWSTR fileName, HANDLE** fileHandle) {
  if (!fileHandle || !fileName) {
    return 2;
  }
  auto handle = CreateFileW(fileName, 0x80000000, 1, 0, 3, 1, 0);
  if (handle == INVALID_HANDLE_VALUE) {
    return 3;
  }
  fileHandle[0] = &handle;
  return 0; // NoError(?)
}

void WINAPI Main() {
  InitProxy();

  if (MH_Initialize() != MH_STATUS::MH_OK) {
    MessageBoxA(NULL, "Failed to initialize MinHook!", "OVRServiceHook (Oculus Ameliorated)", MB_ICONERROR | MB_OK);
  }

  auto verifyLibraryAddress = reinterpret_cast<PBYTE>(util::GetBaseAddress() + 0x162CC0);
  MH_CreateHook(verifyLibraryAddress, VerifyLibraryHook, NULL);
  MH_EnableHook(verifyLibraryAddress);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  if (dwReason == DLL_PROCESS_ATTACH) {
    Main();
  }
  return TRUE;
}

