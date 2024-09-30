#include "rlvr_oaf.h"

const char* oafLogLevelType_ToString(oafLogLevelType log_level_type) {
  switch (log_level_type) {
  case OAF_LOG_LEVEL_TYPE_DISABLED: return " {DISABLED}";
  case OAF_LOG_LEVEL_TYPE_TRACE: return " {TRACE}   ";
  case OAF_LOG_LEVEL_TYPE_DEBUG: return " {DEBUG}   ";
  case OAF_LOG_LEVEL_TYPE_INFO: return " {INFO}    ";
  case OAF_LOG_LEVEL_TYPE_WARNING: return " {WARNING} ";
  case OAF_LOG_LEVEL_TYPE_ERROR: return " {!ERROR!} ";
  default: return " {???}     ";
  }
}

const char* oafInputEventType_GetName(oafInputEventType event_type) {
  switch (event_type) {
  case OAF_INPUT_EVENT_TYPE_CONTROLLER: return "OAF_INPUT_EVENT_TYPE_CONTROLLER";
  case OAF_INPUT_EVENT_TYPE_PROXIMITY_SENSOR: return "OAF_INPUT_EVENT_TYPE_PROXIMITY_SENSOR";
  case OAF_INPUT_EVENT_TYPE_DEVICE_MOTION: return "OAF_INPUT_EVENT_TYPE_DEVICE_MOTION";
  }
  return "Unknown";
}

const char* oafInputControllerEventType_GetName(oafInputControllerEventType event_type) {
  switch (event_type) {
  case OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON: return "OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON";
  }
  return "Unknown";
}

const char* oafHMDEventType_GetName(oafHMDEventType event_type) {
  switch (event_type) {
  case OAF_HMD_EVENT_TYPE_DISPLAY_ATTACHED: return "OAF_HMD_EVENT_TYPE_DISPLAY_ATTACHED";
  case OAF_HMD_EVENT_TYPE_DISPLAY_DETACHED: return "OAF_HMD_EVENT_TYPE_DISPLAY_DETACHED";
  case OAF_HMD_EVENT_TYPE_HMD_ATTACHED: return "OAF_HMD_EVENT_TYPE_HMD_ATTACHED";
  case OAF_HMD_EVENT_TYPE_HMD_AVAILABLE: return "OAF_HMD_EVENT_TYPE_HMD_AVAILABLE";
  case OAF_HMD_EVENT_TYPE_HMD_DETACHED: return "OAF_HMD_EVENT_TYPE_HMD_DETACHED";
  case OAF_HMD_EVENT_TYPE_IAD_CHANGED: return "OAF_HMD_EVENT_TYPE_IAD_CHANGED";
  }
  return "Unknown";
}

rlvr::IOafCallbacks* rlvr::IOafCallbacks::oaf_callbacks = nullptr;

int32_t (*rlvr::oaf_service::OafService_ChangeFocus)(uint32_t process_id) = nullptr;
int32_t (*rlvr::oaf_service::OafService_ChangeInputFocus)(uint32_t process_id, uint64_t a2) = nullptr;
int32_t (*rlvr::oaf_service::OafService_SetModalSystemOverlay)(bool value) = nullptr;
int32_t (*rlvr::oaf_service::OafService_SetDepthBuffersRequestStatus)(bool value) = nullptr;
int32_t (*rlvr::oaf_service::OafService_ActivateHeadset)(const char serial_number[16], int32_t a2) = nullptr;
void (*rlvr::oaf_service::OafService_Log)(const char* category, oafLogLevelType log_level_type, const char* msg) = nullptr;
uint8_t (*rlvr::oaf_service::OafService_SetTrackingMode)(bool value) = nullptr;
uint8_t (*rlvr::oaf_service::OafService_SetAirLinkPairingResult)(bool value, const char serial_number[16]) = nullptr;

void OafService_Log_dummy(const char* category, oafLogLevelType log_level_type, const char* msg) {
  printf("[%s] %s\n", category, msg);
}

void rlvr::oaf_service::OafService_Init(void** functions) {
  OafService_ChangeFocus = decltype(OafService_ChangeFocus)(functions[oafService_ChangeFocus]);
  OafService_ChangeInputFocus = decltype(OafService_ChangeInputFocus)(functions[oafService_ChangeInputFocus]);
  OafService_SetModalSystemOverlay = decltype(OafService_SetModalSystemOverlay)(functions[oafService_SetModalSystemOverlay]);
  OafService_SetDepthBuffersRequestStatus = decltype(OafService_SetDepthBuffersRequestStatus)(functions[oafService_SetDepthBuffersRequestStatus]);
  OafService_ActivateHeadset = decltype(OafService_ActivateHeadset)(functions[oafService_ActivateHeadset]);
#if !IS_LEGACY_RUNTIME
  OafService_Log = decltype(OafService_Log)(functions[oafService_Log]);
#else
  OafService_Log = decltype(OafService_Log)(OafService_Log_dummy);
#endif
  OafService_SetTrackingMode = decltype(OafService_SetTrackingMode)(functions[oafService_SetTrackingMode]);
  OafService_SetAirLinkPairingResult = decltype(OafService_SetAirLinkPairingResult)(functions[oafService_SetAirLinkPairingResult]);
}

void OafCallback_OafServer_Run() { rlvr::IOafCallbacks::oaf_callbacks->OafServer_Run(); }
void OafCallback_OafServer_TearDown() { rlvr::IOafCallbacks::oaf_callbacks->OafServer_TearDown(); }
uint8_t OafCallback_FetchGatekeeper(const char* gk_name) { return rlvr::IOafCallbacks::oaf_callbacks->FetchGatekeeper(gk_name); }
void OafCallback_NewProcess(uint32_t process_id) { rlvr::IOafCallbacks::oaf_callbacks->NewProcess(process_id); }
void OafCallback_GetFPSSettingForHeadset(oafFPSSettingForHeadset* out_value, const char* serial_number) {
  rlvr::IOafCallbacks::oaf_callbacks->GetFPSSettingForHeadset(out_value, serial_number);
}
void OafCallback_Shutdown() { rlvr::IOafCallbacks::oaf_callbacks->Shutdown(); }
void OafCallback_InputEvent(const char* serial_number, oafInputEventType event_type, oafInputControllerEventType controller_event_type, bool value) {
  rlvr::IOafCallbacks::oaf_callbacks->InputEvent(serial_number, event_type, controller_event_type, value);
}
void OafCallback_HMDEvent(const char* serial_number, oafHMDEventType event_type) { rlvr::IOafCallbacks::oaf_callbacks->HMDEvent(serial_number, event_type); }
void OafCallback_SetDefaultHeadset(const char* serial_number) { rlvr::IOafCallbacks::oaf_callbacks->SetDefaultHeadset(serial_number); }
void OafCallback_NotifyAirLinkPairingStart(const char* pairing_code, const char* serial_number) {
  rlvr::IOafCallbacks::oaf_callbacks->NotifyAirLinkPairingStart(pairing_code, serial_number);
}
uint8_t OafCallback_ConnectIpcRemoteHeadsetClient() { return rlvr::IOafCallbacks::oaf_callbacks->ConnectIpcRemoteHeadsetClient(); }

void rlvr::IOafCallbacks::Init(void** callbacks) {
  callbacks[oafCallback_OafServer_Run] = (void*)OafCallback_OafServer_Run;
  callbacks[oafCallback_OafServer_TearDown] = (void*)OafCallback_OafServer_TearDown;
  callbacks[oafCallback_FetchGatekeeper] = (void*)OafCallback_FetchGatekeeper;
  callbacks[oafCallback_NewProcess] = (void*)OafCallback_NewProcess;
  callbacks[oafCallback_GetFPSSettingForHeadset] = (void*)OafCallback_GetFPSSettingForHeadset;
  callbacks[oafCallback_Shutdown] = (void*)OafCallback_Shutdown;
  callbacks[oafCallback_InputEvent] = (void*)OafCallback_InputEvent;
  callbacks[oafCallback_HMDEvent] = (void*)OafCallback_HMDEvent;
  callbacks[oafCallback_SetDefaultHeadset] = (void*)OafCallback_SetDefaultHeadset;
  callbacks[oafCallback_NotifyAirLinkPairingStart] = (void*)OafCallback_NotifyAirLinkPairingStart;
  callbacks[oafCallback_ConnectIpcRemoteHeadsetClient] = (void*)OafCallback_ConnectIpcRemoteHeadsetClient;
}
