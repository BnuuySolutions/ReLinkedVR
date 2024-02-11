#include <OVR_OAF.hpp>
#include <windows.h>
#include <cstdint>
#include <cstdio>

uint32_t Oaf_NewProcess(uint32_t pid) {
  Oaf_OVRService_SetTrackingMode(true);
  Oaf_OVRService_SetModalSystemOverlay(true);
  Oaf_OVRService_SetDepthBuffersRequestStatus(false);
  Oaf_OVRService_ChangeFocus(pid);
  Oaf_OVRService_ChangeInputFocus(pid, 0);
  return 0;
}

uint32_t Oaf_HMDEvent() {
  static const char* serialNumber = "WMHD3032200CBR";
  static char serialBlock[16];
  memset(serialBlock, 0, 16);
  memcpy_s(serialBlock, 16, serialNumber, strlen(serialNumber));
  static bool isActive = false;
  if (!isActive) {
    Oaf_OVRService_ActivateHeadset(serialBlock, 2);
    isActive = true;
  }
  return 0;
}

OVR_EXPORT void OculusAppFrameworkLoad() {}
OVR_EXPORT bool OculusAppFrameworkInit(void* funcTable) {
  Oaf_Init(funcTable);
  return false;
}

OVR_EXPORT void OculusAppFrameworkSetCallbacks(void* callbackTable) {
  void** callbacks = reinterpret_cast<void**>(static_cast<char*>(callbackTable) + sizeof(uint64_t)); // get to the actual callbacks, ignore table size.
  callbacks[OAF_NEWPROCESS_CALLBACK_ID] = Oaf_NewProcess;
  callbacks[OAF_HMDEVENT_CALLBACK_ID] = Oaf_HMDEvent;
}
