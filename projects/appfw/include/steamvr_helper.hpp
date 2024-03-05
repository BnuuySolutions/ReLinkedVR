#pragma once
#include "util.hpp"
#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

// port from old codebase to hold us over.
class steamvr_helper {
private:
  static HANDLE hProcess;
  static HANDLE hThread;

  static bool IsProcessOpen(HANDLE hProcess) {
    DWORD dwExitCode;
    if (GetExitCodeProcess(hProcess, &dwExitCode) && dwExitCode == STILL_ACTIVE) {
      return true;
    }
    return false;
  }

public:
  static bool IsProcessVRServer(uint32_t pid) {
    HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
    if (hproc) {
      wchar_t buf[MAX_PATH];
      if (GetModuleFileNameExW(hproc, 0, buf, MAX_PATH)) {
        if (wcsstr(buf, L"vrserver.exe")) {
          return true;
        }
      }
    }
    return false;
  }

  static void OpenSteamVR() {
    if (IsProcessOpen(hProcess)) {
      return;
    }

    std::wstring localAppDataDir = util::GetLocalAppDataDir();
    std::wstring openVRPathsPath = util::GetConcatPath_Utf16(localAppDataDir, L"openvr\\openvrpaths.vrpath");
    if (fs::exists(openVRPathsPath)) {
      json openVRPathsJSON = util::GetJSONFile(openVRPathsPath);
      json openVRPathsRuntimes = openVRPathsJSON["runtime"];
      if ((!openVRPathsRuntimes.is_null() && openVRPathsRuntimes.is_array()) && openVRPathsRuntimes.size() > 0) {
        std::string openVRPathsRuntime = openVRPathsRuntimes[0];
        std::string vrStartupPath = util::GetConcatPath_Utf8(openVRPathsRuntime, "bin\\win64\\vrstartup.exe");
        if (fs::exists(vrStartupPath)) {
          STARTUPINFOA si{};
          PROCESS_INFORMATION pi{};
          if (CreateProcessA(
            vrStartupPath.c_str(),
            nullptr,
            nullptr,
            nullptr,
            false,
            0,
            nullptr,
            nullptr,
            &si,
            &pi)) {
            hProcess = pi.hProcess;
            hThread = pi.hThread;
          }
        }
      }
    }
  }
};
