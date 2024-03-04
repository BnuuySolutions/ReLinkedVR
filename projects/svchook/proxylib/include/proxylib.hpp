#include <windows.h>
#include <cstdio>

extern "C" FARPROC GetFileVersionInfoA_orig = NULL;
extern "C" FARPROC GetFileVersionInfoByHandle_orig = NULL;
extern "C" FARPROC GetFileVersionInfoExA_orig = NULL;
extern "C" FARPROC GetFileVersionInfoExW_orig = NULL;
extern "C" FARPROC GetFileVersionInfoSizeA_orig = NULL;
extern "C" FARPROC GetFileVersionInfoSizeExA_orig = NULL;
extern "C" FARPROC GetFileVersionInfoSizeExW_orig = NULL;
extern "C" FARPROC GetFileVersionInfoSizeW_orig = NULL;
extern "C" FARPROC GetFileVersionInfoW_orig = NULL;
extern "C" FARPROC VerFindFileA_orig = NULL;
extern "C" FARPROC VerFindFileW_orig = NULL;
extern "C" FARPROC VerInstallFileA_orig = NULL;
extern "C" FARPROC VerInstallFileW_orig = NULL;
extern "C" FARPROC VerLanguageNameA_orig = NULL;
extern "C" FARPROC VerLanguageNameW_orig = NULL;
extern "C" FARPROC VerQueryValueA_orig = NULL;
extern "C" FARPROC VerQueryValueW_orig = NULL;

extern "C" void GetFileVersionInfoA_proxylib();
extern "C" void GetFileVersionInfoByHandle_proxylib();
extern "C" void GetFileVersionInfoExA_proxylib();
extern "C" void GetFileVersionInfoExW_proxylib();
extern "C" void GetFileVersionInfoSizeA_proxylib();
extern "C" void GetFileVersionInfoSizeExA_proxylib();
extern "C" void GetFileVersionInfoSizeExW_proxylib();
extern "C" void GetFileVersionInfoSizeW_proxylib();
extern "C" void GetFileVersionInfoW_proxylib();
extern "C" void VerFindFileA_proxylib();
extern "C" void VerFindFileW_proxylib();
extern "C" void VerInstallFileA_proxylib();
extern "C" void VerInstallFileW_proxylib();
extern "C" void VerLanguageNameA_proxylib();
extern "C" void VerLanguageNameW_proxylib();
extern "C" void VerQueryValueA_proxylib();
extern "C" void VerQueryValueW_proxylib();

uintptr_t __proxylib_get_base_address() {
  return reinterpret_cast<uintptr_t>(GetModuleHandle(0));
}

bool __proxylib_init() {
  wchar_t sysdir[MAX_PATH];
  if (!GetSystemDirectoryW(sysdir, MAX_PATH)) {
    return false;
  }
  wchar_t dllpath[MAX_PATH];
  wsprintfW(dllpath, L"%s/%s", sysdir, L"version.dll");
  auto dllhandle = LoadLibraryW(dllpath);
  if (dllhandle) {
    GetFileVersionInfoA_orig = GetProcAddress(dllhandle, "GetFileVersionInfoA");
    GetFileVersionInfoByHandle_orig = GetProcAddress(dllhandle, "GetFileVersionInfoByHandle");
    GetFileVersionInfoExA_orig = GetProcAddress(dllhandle, "GetFileVersionInfoExA");
    GetFileVersionInfoExW_orig = GetProcAddress(dllhandle, "GetFileVersionInfoExW");
    GetFileVersionInfoSizeA_orig = GetProcAddress(dllhandle, "GetFileVersionInfoSizeA");
    GetFileVersionInfoSizeExA_orig = GetProcAddress(dllhandle, "GetFileVersionInfoSizeExA");
    GetFileVersionInfoSizeExW_orig = GetProcAddress(dllhandle, "GetFileVersionInfoSizeExW");
    GetFileVersionInfoSizeW_orig = GetProcAddress(dllhandle, "GetFileVersionInfoSizeW");
    GetFileVersionInfoW_orig = GetProcAddress(dllhandle, "GetFileVersionInfoW");
    VerFindFileA_orig = GetProcAddress(dllhandle, "VerFindFileA");
    VerFindFileW_orig = GetProcAddress(dllhandle, "VerFindFileW");
    VerInstallFileA_orig = GetProcAddress(dllhandle, "VerInstallFileA");
    VerInstallFileW_orig = GetProcAddress(dllhandle, "VerInstallFileW");
    VerLanguageNameA_orig = GetProcAddress(dllhandle, "VerLanguageNameA");
    VerLanguageNameW_orig = GetProcAddress(dllhandle, "VerLanguageNameW");
    VerQueryValueA_orig = GetProcAddress(dllhandle, "VerQueryValueA");
    VerQueryValueW_orig = GetProcAddress(dllhandle, "VerQueryValueW");
    return true;
  }
  return false;
}
