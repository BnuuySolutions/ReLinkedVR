// Used for emulating "OculusAppFramework.dll", without OVRService.

#include <windows.h>
#include <cstdio>
#include "func_table.h"

enum oafCallbackType {
  oafCallback_OafServer_TestConnection = 0,
  oafCallback_OafServer_UploadCrashReports = 1,
  oafCallback_OafServer_Run = 2
};

void (*OculusAppFrameworkLoad)() = nullptr;
bool (*OculusAppFrameworkInit)(void* functionTable) = nullptr;
void (*OculusAppFrameworkSetCallbacks)(void* callbackTable) = nullptr;

void (*OafCallback_OafServer_TestConnection)() = nullptr;
void (*OafCallback_OafServer_UploadCrashReports)() = nullptr;
void (*OafCallback_OafServer_Run)() = nullptr;

const char* LogLevel_ToString(int8_t log_level) {
  switch (log_level) {
  case 0: return " {DISABLED}";
  case 1: return " {TRACE}   ";
  case 2: return " {DEBUG}   ";
  case 3: return " {INFO}    ";
  case 4: return " {WARNING} ";
  case 5: return " {!ERROR!} ";
  default: return " {???}     ";
  }
}

void OafService_Log(char* class_name, int8_t log_level, char* msg, __int64 a4, __int32 a5) {
  //if (a4 != 0 || a5 != 0) {
  //  printf("%i %i\n", a4, a5);
  //}
  printf("%s[%s] %s\n", LogLevel_ToString(log_level), class_name, msg);
}

void nullsub() {}

struct FuncTable<102> g_oafservice_table = {
  .total_size = FUNC_TABLE_CALC_SIZE(102),
  .elements = {},
  .unk_0 = UINT32_MAX
};

struct FuncTable<78> g_oafcallback_table = {
  .total_size = FUNC_TABLE_CALC_SIZE(78),
  .elements = {},
  .unk_0 = UINT32_MAX
};

int main() {
  HINSTANCE hlib = LoadLibraryW(L"OculusAppFramework.dll");
  if (!hlib) {
    printf("OAF failed to load.\n");
    return 0;
  }
  OculusAppFrameworkLoad = (decltype(OculusAppFrameworkLoad))GetProcAddress(hlib, "OculusAppFrameworkLoad");
  OculusAppFrameworkInit = (decltype(OculusAppFrameworkInit))GetProcAddress(hlib, "OculusAppFrameworkInit");
  OculusAppFrameworkSetCallbacks = (decltype(OculusAppFrameworkSetCallbacks))GetProcAddress(hlib, "OculusAppFrameworkSetCallbacks");
  if (!OculusAppFrameworkLoad || !OculusAppFrameworkInit || !OculusAppFrameworkSetCallbacks) {
    printf("OAF is missing exported functions.\n");
    return 0;
  }
  OculusAppFrameworkLoad();
  for (int32_t i = 0; i < 102; i++)
    g_oafservice_table.elements[i] = (void*)nullsub;
  g_oafservice_table.elements[46] = (void*)OafService_Log;
  if (OculusAppFrameworkInit(&g_oafservice_table)) {
    printf("OAF initialization failed.\n");
    return 0;
  }
  OculusAppFrameworkSetCallbacks(&g_oafcallback_table);
  OafCallback_OafServer_TestConnection = (decltype(OafCallback_OafServer_TestConnection))g_oafcallback_table.elements[oafCallback_OafServer_TestConnection];
  OafCallback_OafServer_UploadCrashReports = (decltype(OafCallback_OafServer_UploadCrashReports))g_oafcallback_table.elements[oafCallback_OafServer_UploadCrashReports];
  OafCallback_OafServer_Run = (decltype(OafCallback_OafServer_Run))g_oafcallback_table.elements[oafCallback_OafServer_Run];
  OafCallback_OafServer_UploadCrashReports();
  OafCallback_OafServer_TestConnection();
  OafCallback_OafServer_Run();
  return 0;
}
