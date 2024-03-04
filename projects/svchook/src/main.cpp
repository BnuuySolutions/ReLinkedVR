#include "proxylib.hpp"
#include <windows.h>
#include <MinHook.h>

// Disables library validation inside OVRService.
char ValidateLibraryHook(LPCWSTR filename, HANDLE** handles) {
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

int main() {
  if (MH_Initialize() != MH_STATUS::MH_OK) {
    MessageBoxW(NULL, L"Failed to initialize MinHook!", L"svchook", MB_ICONERROR | MB_OK);
    return -1;
  }

  auto p_ValidateLibrary = reinterpret_cast<PBYTE>(__proxylib_get_base_address() + 0x165F70);
  MH_CreateHook(p_ValidateLibrary, ValidateLibraryHook, NULL);
  MH_EnableHook(p_ValidateLibrary);
  return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
  int result = -1;
  if (dwReason == DLL_PROCESS_ATTACH) {
    if (!__proxylib_init()) {
      MessageBoxW(NULL, L"Failed to initialize proxylib!", L"svchook", MB_ICONERROR | MB_OK);
      return FALSE;
    }
    result = main();
  }
  return result == 0 ? TRUE : FALSE;
}
