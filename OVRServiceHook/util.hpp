#pragma once
#include <windows.h>
#include <shlobj.h>
#include <fstream>
#include <sstream>
#include <string>

class util {
public:
  static uintptr_t GetBaseAddress() {
    return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
  }

  static std::wstring GetConcatPath_Utf16(const std::wstring& first, const std::wstring& second) {
    std::wstring temp = first;
    if (first[first.length()] != '\\') {
      temp += '\\';
      return temp + second;
    } else {
      return first + second;
    }
  }

  static std::wstring GetSysDir() {
    wchar_t sysDir[MAX_PATH];
    if (!GetSystemDirectoryW(sysDir, MAX_PATH)) {
      MessageBoxA(NULL, "Failed to get system directory!", "OVRServiceHook (Oculus Ameliorated)", MB_ICONERROR | MB_OK);
    }
    return sysDir;
  }

  static bool FileExists(LPCWSTR lpPath) {
    DWORD attrib = GetFileAttributesW(lpPath);
    return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
  }
};
