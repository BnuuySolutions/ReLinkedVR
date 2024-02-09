#pragma once
#include <windows.h>

// The "version.dll" proxy stuff was copied from MelonLoader, long, long ago...
// Only because it was unfortunately cleaner than anything else I could make, for now.
// MelonLoader is licensed under "Apache License 2.0", you should check their license here:
// - https://github.com/LavaGang/MelonLoader/blob/master/LICENSE.md
// The only modifications I made was changing the _<Name>, and changing how the library was loaded.

extern "C" void GetFileVersionInfoA_OVR();
extern "C" void GetFileVersionInfoByHandle_OVR();
extern "C" void GetFileVersionInfoExA_OVR();
extern "C" void GetFileVersionInfoExW_OVR();
extern "C" void GetFileVersionInfoSizeA_OVR();
extern "C" void GetFileVersionInfoSizeExA_OVR();
extern "C" void GetFileVersionInfoSizeExW_OVR();
extern "C" void GetFileVersionInfoSizeW_OVR();
extern "C" void GetFileVersionInfoW_OVR();
extern "C" void VerFindFileA_OVR();
extern "C" void VerFindFileW_OVR();
extern "C" void VerInstallFileA_OVR();
extern "C" void VerInstallFileW_OVR();
extern "C" void VerLanguageNameA_OVR();
extern "C" void VerLanguageNameW_OVR();
extern "C" void VerQueryValueA_OVR();
extern "C" void VerQueryValueW_OVR();

extern "C" FARPROC GetFileVersionInfoA_o = NULL;
extern "C" FARPROC GetFileVersionInfoByHandle_o = NULL;
extern "C" FARPROC GetFileVersionInfoExA_o = NULL;
extern "C" FARPROC GetFileVersionInfoExW_o = NULL;
extern "C" FARPROC GetFileVersionInfoSizeA_o = NULL;
extern "C" FARPROC GetFileVersionInfoSizeExA_o = NULL;
extern "C" FARPROC GetFileVersionInfoSizeExW_o = NULL;
extern "C" FARPROC GetFileVersionInfoSizeW_o = NULL;
extern "C" FARPROC GetFileVersionInfoW_o = NULL;
extern "C" FARPROC VerFindFileA_o = NULL;
extern "C" FARPROC VerFindFileW_o = NULL;
extern "C" FARPROC VerInstallFileA_o = NULL;
extern "C" FARPROC VerInstallFileW_o = NULL;
extern "C" FARPROC VerLanguageNameA_o = NULL;
extern "C" FARPROC VerLanguageNameW_o = NULL;
extern "C" FARPROC VerQueryValueA_o = NULL;
extern "C" FARPROC VerQueryValueW_o = NULL;
