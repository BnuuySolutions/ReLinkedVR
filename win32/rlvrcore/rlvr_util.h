#pragma once

#include <windows.h>
#include <shlobj.h>
#include <psapi.h>
#include <cstdint>
#include <cstdio>
#include <string>

namespace rlvr {

class Util {
private:
  static bool MaskCompare(void* buf, const char* pattern, const char* mask);
  static uint8_t* FindPattern(void* base, size_t size, const char* pattern, const char* mask);

public:
  inline static void* GetBaseAddress(const wchar_t* module_name) { return (void*)GetModuleHandleW(module_name); }
  inline static void* GetBaseAddress() { return GetBaseAddress(0); }

  static uint8_t* FindPattern(HMODULE module_handle, const char* pattern, const char* mask);
  inline static uint8_t* FindPattern(const wchar_t* module_name, const char* pattern, const char* mask) {
    return FindPattern(GetModuleHandleW(module_name), pattern, mask);
  }
  inline static uint8_t* FindPattern(const char* pattern, const char* mask) {
    return FindPattern((wchar_t*)0, pattern, mask);
  }

  static std::string GetConcatPath_Utf8(const std::string& first, const std::string& second);
  static std::wstring GetConcatPath_Utf16(const std::wstring& first, const std::wstring& second);

  static std::wstring GetLocalAppDataDir();
};

}
