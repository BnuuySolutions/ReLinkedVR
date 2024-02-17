#include <OVR_OAF.hpp>
#include <windows.h>
#include <psapi.h>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include "util.hpp"

namespace fs = std::filesystem;

class SteamVRHelper {
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
    HANDLE hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,FALSE, pid);
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
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
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

HANDLE SteamVRHelper::hProcess;
HANDLE SteamVRHelper::hThread;

uint32_t Oaf_NewProcess(uint32_t pid) {
  // TODO(Kaitlyn): Doesn't work anyways.
  //if (pid == 0) {
  //  Oaf_OVRService_SetTrackingMode(false);
  //  return 0;
  //}

  if (SteamVRHelper::IsProcessVRServer(pid)) {
    Oaf_OVRService_SetTrackingMode(true);
    Oaf_OVRService_SetModalSystemOverlay(false);
    Oaf_OVRService_SetDepthBuffersRequestStatus(false);
    Oaf_OVRService_ChangeFocus(pid);
    Oaf_OVRService_ChangeInputFocus(pid, 0);
  }
  return 0;
}

uint32_t Oaf_InputEvent(const char* serialNumber, oafInputEventType eventType, oafInputControllerEventType controllerEventType, ovrBool value) {
  //printf("Oaf_InputEvent: serial = %s, eventType = %i, controllerEventType = %i, value = %i\n", serialNumber, eventType, controllerEventType, value);
  switch (eventType) {
  case OAF_INPUT_EVENT_TYPE_CONTROLLER:
    if (controllerEventType == OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON && value == ovrTrue) {
      SteamVRHelper::OpenSteamVR();
    }
    break;
  }
  return 0;
}

uint32_t Oaf_SetDefaultHeadset(const char* serialNumber) {
  Oaf_OVRService_ActivateHeadset(serialNumber, 2);
  return 0;
}

uint32_t Oaf_HMDEvent(const char* serialNumber, oafHMDEventType eventType) {
  if (eventType == OAF_HMD_EVENT_TYPE_HMD_ATTACHED) {
    Oaf_SetDefaultHeadset(serialNumber);
  }
  return 0;
}

OVR_EXPORT void OculusAppFrameworkLoad() {}
OVR_EXPORT bool OculusAppFrameworkInit(void* functionTable) {
  void** functions = reinterpret_cast<void**>(static_cast<char*>(functionTable) + sizeof(uint64_t));
  OculusAppFrameworkInitFunctions(functions);
  return false;
}

OVR_EXPORT void OculusAppFrameworkSetCallbacks(void* callbackTable) {
  void** callbacks = reinterpret_cast<void**>(static_cast<char*>(callbackTable) + sizeof(uint64_t)); // get to the actual callbacks, ignore table size.
  callbacks[OAF_NEWPROCESS_CALLBACK_ID] = Oaf_NewProcess;
  callbacks[OAF_INPUTEVENT_CALLBACK_ID] = Oaf_InputEvent;
  callbacks[OAF_HMDEVENT_CALLBACK_ID] = Oaf_HMDEvent;
  callbacks[OAF_SETDEFAULTHEADSET_CALLBACK_ID] = Oaf_SetDefaultHeadset;
}
