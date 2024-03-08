#include "ipc_server.hpp"
#include "airlink_helper.hpp"
#include "steamvr_helper.hpp"
#include <ovr_oaf.hpp>

HANDLE steamvr_helper::hProcess;
HANDLE steamvr_helper::hThread;

uint32_t Oaf_NewProcess(uint32_t pid) {
  // TODO(Kaitlyn): Doesn't work anyways.
  //if (pid == 0) {
  //  Oaf_OVRService_SetTrackingMode(false);
  //  return 0;
  //}

  if (steamvr_helper::IsProcessVRServer(pid)) {
    Oaf_OVRService_SetTrackingMode(true);
    Oaf_OVRService_SetModalSystemOverlay(false);
    Oaf_OVRService_SetDepthBuffersRequestStatus(false);
    Oaf_OVRService_ChangeFocus(pid);
    Oaf_OVRService_ChangeInputFocus(pid, 0);
  }
  return 0;
}

uint32_t Oaf_InputEvent(const char* serialNumber, oafInputEventType eventType, oafInputControllerEventType controllerEventType, ovrBool value) {
  //printf("Oaf_InputEvent: serialNumber = %s, eventType = %i, controllerEventType = %i, value = %i\n", serialNumber, eventType, controllerEventType, value);
  switch (eventType) {
  case OAF_INPUT_EVENT_TYPE_CONTROLLER:
    if (controllerEventType == OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON && value == true) {
      steamvr_helper::OpenSteamVR();
    }
    break;
  }
  return 0;
}

uint32_t Oaf_SetDefaultHeadset(const char* serialNumber) {
  printf("Oaf_SetDefaultHeadset: serialNumber = %s\n", serialNumber);
  Oaf_OVRService_ActivateHeadset(serialNumber, 2);
  return 0;
}

uint32_t Oaf_HMDEvent(const char* serialNumber, oafHMDEventType eventType) {
  //printf("Oaf_HMDEvent: serialNumber = %s, eventType = %i\n", serialNumber, eventType);
  if (eventType == OAF_HMD_EVENT_TYPE_HMD_ATTACHED) {
    Oaf_SetDefaultHeadset(serialNumber);
  }
  return 0;
}

uint32_t Oaf_NotifyAirLinkPairingStart(const char* pairingCode, const char* serialNumber) {
  printf("Oaf_NotifyAirLinkPairingStart: pairingCode = %s, serialNumber = %s\n", pairingCode, serialNumber);
  airlink_helper::StartPairing(pairingCode, serialNumber);
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
  callbacks[OAF_NOTIFYAIRLINKPAIRINGSTART_CALLBACK_ID] = Oaf_NotifyAirLinkPairingStart;
}
