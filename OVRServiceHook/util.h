#pragma once
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <string>
#include "proxy.h"

namespace oculus_ameliorated {

class util {
public:
  static void InitProxy() {
    HMODULE hVersionDll = LoadLibraryA(GetConcatPath(GetSystemDir(), "version.dll").c_str());
    if (hVersionDll) {
      GetFileVersionInfoA_o = GetProcAddress(hVersionDll, "GetFileVersionInfoA");
      GetFileVersionInfoByHandle_o = GetProcAddress(hVersionDll, "GetFileVersionInfoByHandle");
      GetFileVersionInfoExA_o = GetProcAddress(hVersionDll, "GetFileVersionInfoExA");
      GetFileVersionInfoExW_o = GetProcAddress(hVersionDll, "GetFileVersionInfoExW");
      GetFileVersionInfoSizeA_o = GetProcAddress(hVersionDll, "GetFileVersionInfoSizeA");
      GetFileVersionInfoSizeExA_o = GetProcAddress(hVersionDll, "GetFileVersionInfoSizeExA");
      GetFileVersionInfoSizeExW_o = GetProcAddress(hVersionDll, "GetFileVersionInfoSizeExW");
      GetFileVersionInfoSizeW_o = GetProcAddress(hVersionDll, "GetFileVersionInfoSizeW");
      GetFileVersionInfoW_o = GetProcAddress(hVersionDll, "GetFileVersionInfoW");
      VerFindFileA_o = GetProcAddress(hVersionDll, "VerFindFileA");
      VerFindFileW_o = GetProcAddress(hVersionDll, "VerFindFileW");
      VerInstallFileA_o = GetProcAddress(hVersionDll, "VerInstallFileA");
      VerInstallFileW_o = GetProcAddress(hVersionDll, "VerInstallFileW");
      VerLanguageNameA_o = GetProcAddress(hVersionDll, "VerLanguageNameA");
      VerLanguageNameW_o = GetProcAddress(hVersionDll, "VerLanguageNameW");
      VerQueryValueA_o = GetProcAddress(hVersionDll, "VerQueryValueA");
      VerQueryValueW_o = GetProcAddress(hVersionDll, "VerQueryValueW");
    } else {
      MessageBoxA(NULL, "Failed to load original version.dll!", "OVRServiceHook (Oculus Ameliorated)", MB_ICONERROR | MB_OK);
    }
  }

  static uintptr_t GetBaseAddress() {
    return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
  }

  static std::string GetConcatPath(const std::string& sFirst, const std::string& sSecond) {
    std::string sTemp = sFirst;

    if (sFirst[sFirst.length()] != '\\') {
      sTemp += '\\';
      return sTemp + sSecond;
    } else {
      return sFirst + sSecond;
    }
  }

  static std::string GetSystemDir() {
    char sSystemDir[MAX_PATH];
    if (!GetSystemDirectoryA(sSystemDir, MAX_PATH)) {
      MessageBoxA(NULL, "Failed to get system directory!", "OVRServiceHook (Oculus Ameliorated)", MB_ICONERROR | MB_OK);
    }

    return sSystemDir;
  }

  static BOOL FileExists(LPCWSTR lpPath) {
    DWORD dwAttrib = GetFileAttributesW(lpPath);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
  }
};

}
