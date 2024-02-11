#pragma once
#include <windows.h>
#include <string>

class util {
public:
  static uintptr_t GetBaseAddress() {
    return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
  }

  static std::string GetConcatPath(const std::string& first, const std::string& second) {
    std::string temp = first;
    if (first[first.length()] != '\\') {
      temp += '\\';
      return temp + second;
    } else {
      return first + second;
    }
  }

  static std::string GetSysDir() {
    char sysDir[MAX_PATH];
    if (!GetSystemDirectoryA(sysDir, MAX_PATH)) {
      MessageBoxA(NULL, "Failed to get system directory!", "OVRServiceHook (Oculus Ameliorated)", MB_ICONERROR | MB_OK);
    }
    return sysDir;
  }

  static bool FileExists(LPCWSTR lpPath) {
    DWORD attrib = GetFileAttributesW(lpPath);
    return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
  }
};
