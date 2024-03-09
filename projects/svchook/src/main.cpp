#include "proxylib.hpp"
#include <windows.h>
#include <MinHook.h>

// Disables library validation inside OVRService.
static char ValidateLibraryHook(LPCWSTR filename, HANDLE** handles) {
  if (!handles || !filename) {
    return 2;
  }
  auto filehandle = CreateFileW(filename, 0x80000000, 1, 0, 3, 1, 0);
  if (filehandle == INVALID_HANDLE_VALUE) {
    return 3;
  }
  handles[0] = &filehandle;
  return 0; // NoError(?)
}

int WINAPI main() {
  if (MH_Initialize() != MH_STATUS::MH_OK) {
    MessageBoxW(nullptr, L"Failed to initialize MinHook!", L"svchook", MB_ICONERROR | MB_OK);
    return -1;
  }

  auto validateLibraryAddr = static_cast<LPVOID>(proxylib::FindPattern("\x4C\x8B\xDC\x55\x57\x49\x8D\xAB\x98\xF9\xFF\xFF\x48\x81\xEC\x58\x07\x00\x00\x48\x8B\x05\x00\x00\x6A\x00\x48\x33\xC4\x48\x89\x85\x20\x06\x00\x00\x48\x89\x55\xA8\x48\x8B\xF9\x48\x85\xD2", "xxxxxxxxxxxxxxxxxxxxxx??xxxxxxxxxxxxxxxxxxxxxx"));
  MH_CreateHook(validateLibraryAddr, ValidateLibraryHook, nullptr);
  MH_EnableHook(validateLibraryAddr);
  return 0;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  int result = -1;
  if (dwReason == DLL_PROCESS_ATTACH) {
    if (!proxylib::Init()) {
      MessageBoxW(nullptr, L"Failed to initialize proxylib!", L"svchook", MB_ICONERROR | MB_OK);
      return false;
    }
    result = main();
  }
  return result == 0 ? true : false;
}
