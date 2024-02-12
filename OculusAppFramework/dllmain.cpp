#include <OVR_OAF.hpp>
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

uint32_t Oaf_NewProcess(uint32_t pid) {
  Oaf_OVRService_SetTrackingMode(true);
  Oaf_OVRService_SetModalSystemOverlay(true);
  Oaf_OVRService_SetDepthBuffersRequestStatus(false);
  Oaf_OVRService_ChangeFocus(pid);
  Oaf_OVRService_ChangeInputFocus(pid, 0);
  return 0;
}

uint32_t Oaf_InputEvent(const char* serialNumber, oafInputEventType eventType, oafInputControllerEventType controllerEventType, ovrBool value) {
  printf("Oaf_InputEvent: serial = %s, eventType = %i, controllerEventType = %i, value = %i\n", serialNumber, eventType, controllerEventType, value);
  return 0;
}

// Is a2 the type of event?
uint32_t Oaf_HMDEvent(const char* serialNumber, uint8_t a2, uint8_t a3) {
  /*if (IsDebuggerPresent()) */ //DebugBreak();
  printf("Oaf_HMDEvent: %s %i %i\n", serialNumber, a2, a3);
  static bool isActive = false;
  if (!isActive) {
    Oaf_OVRService_ActivateHeadset(serialNumber, 2);
    isActive = true;
  }
  return 0;
}

uint32_t Oaf_SetDefaultHeadset(const char* serialNumber) {
  //Oaf_OVRService_ActivateHeadset(serialNumber, 2);
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
