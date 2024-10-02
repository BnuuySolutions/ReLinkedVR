#include "oaf_callbacks.h"
#include "openvr_helper.h"
#include <filesystem>
#include <format>

using namespace rlvr;
using namespace rlvr::appfw;
using namespace rlvr::oaf_service;

using rlvr_util = rlvr::Util;
using appfw_util = rlvr::appfw::Util;

namespace fs = std::filesystem;

rlvr::appfw::OafCallbacks::OafCallbacks() {
  // init config
  std::wstring appdata_dir = rlvr::Util::GetLocalAppDataDir();
  std::wstring rlvrsettings_path = rlvr::Util::GetConcatPath_Utf16(appdata_dir, L"ReLinked VR\\RLVRSettings.json");
  if (fs::exists(rlvrsettings_path)) {
    _rlvrsettings_json = rlvr::appfw::Util::GetJSONFile(rlvrsettings_path);
    json rlvrsettings_enablesteamvrautostart = _rlvrsettings_json["enableSteamVRAutoStart"];
    if (rlvrsettings_enablesteamvrautostart.is_boolean()) {
      _enable_steamvr_autostart = rlvrsettings_enablesteamvrautostart;
    }
    json rlvrsettings_usecustomfps = _rlvrsettings_json["useCustomFps"];
    if (rlvrsettings_usecustomfps.is_boolean()) {
      _use_custom_fps = rlvrsettings_usecustomfps;
    }
    json rlvrsettings_customfps = _rlvrsettings_json["customFps"];
    if (rlvrsettings_customfps.is_number_integer()) {
      _custom_fps = rlvrsettings_customfps;
    }
    _is_rlvrsettings_loaded = true;
  }

  _enabled_gks.push_back("oculus_compositor_phase_sync");
  _enabled_gks.push_back("oculus_compositor_asw_toggle");
  _enabled_gks.push_back("oculus_link_allow_stinson_120fps"); // Required to make Q3 120 FPS work over Link.
  _enabled_gks.push_back("oculus_allow_hevc_on_amd");

  if (_is_rlvrsettings_loaded) {
    json rlvrsettings_enabledgks = _rlvrsettings_json["enabledGks"];
    if (rlvrsettings_enabledgks.is_array()) {
      for (auto& it : rlvrsettings_enabledgks) {
        if (it.is_string()) {
          _enabled_gks.push_back(it);
        }
      }
    }
  }
}

// TODO(Kaitlyn): IPC
void rlvr::appfw::OafCallbacks::OafServer_Run() {}
void rlvr::appfw::OafCallbacks::OafServer_TearDown() {}

uint8_t rlvr::appfw::OafCallbacks::FetchGatekeeper(const char* gk_name) {
  //OAF_SERVICE_LOG("RLVR:Appfw", OAF_LOG_LEVEL_TYPE_INFO,
  //  "Oaf_FetchGatekeeper: gk_name = {}",
  //  gk_name);
  for (auto it : _enabled_gks) {
    if (strncmp(gk_name, it.c_str(), it.length()) == 0) return 0;
  }
  //_unhandled_gks.push_back(gkName);
  return 2;
}

void rlvr::appfw::OafCallbacks::NewProcess(uint32_t process_id) {
  // TODO(Kaitlyn): Doesn't work anyways.
  //if (processId == 0) {
  //  OafFunction_SetTrackingMode(false);
  //  return;
  //}

  // Restrict to just SteamVR/vrserver.
  if (OpenVRHelper::IsProcessVRServer(process_id)) {
    OafService_SetTrackingMode(true);
    OafService_SetModalSystemOverlay(false);
    OafService_SetDepthBuffersRequestStatus(false);
    OafService_ChangeFocus(process_id);
    OafService_ChangeInputFocus(process_id, 0);
  }
}

void rlvr::appfw::OafCallbacks::GetFPSSettingForHeadset(oafFPSSettingForHeadset* out_value, const char* serial_number) {
  out_value->fps = _custom_fps;
  out_value->use_fps_setting = _use_custom_fps;
}

void rlvr::appfw::OafCallbacks::Shutdown() {} // TODO(Kaitlyn): IPC

void rlvr::appfw::OafCallbacks::InputEvent(const char* serial_number, oafInputEventType event_type, oafInputControllerEventType controller_event_type, bool value) {
  //OAF_SERVICE_LOG("RLVR:Appfw", OAF_LOG_LEVEL_TYPE_INFO,
  //  "Oaf_InputEvent: serial_number = {}, event_type = {}, controller_event_type = {}, value = {}",
  //  serial_number, (int32_t)event_type, (int32_t)controller_event_type, (int32_t)value);
  switch (event_type) {
  case OAF_INPUT_EVENT_TYPE_CONTROLLER:
    if (controller_event_type == OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON && value == true) {
      OpenVRHelper::RunStartup();
    }
    break;
  case OAF_INPUT_EVENT_TYPE_PROXIMITY_SENSOR:
  case OAF_INPUT_EVENT_TYPE_DEVICE_MOTION:
    break;
  }
}

void rlvr::appfw::OafCallbacks::HMDEvent(const char* serial_number, oafHMDEventType event_type) {
  //const char* event_type_name = oafHMDEventType_GetName(event_type);
  //OAF_SERVICE_LOG("RLVR:Appfw", OAF_LOG_LEVEL_TYPE_INFO,
  //  "Oaf_HMDEvent: serial_number = {}, event_type = {} ({})",
  //  serial_number, event_type_name, (int32_t)event_type);
  if (event_type == OAF_HMD_EVENT_TYPE_DISPLAY_ATTACHED) {
    OafService_ActivateHeadset(serial_number, 2);
    if (_enable_steamvr_autostart) {
      OpenVRHelper::RunStartup();
    }
  }
}

void rlvr::appfw::OafCallbacks::SetDefaultHeadset(const char* serial_number) {
  OAF_SERVICE_LOG("RLVR:Appfw", OAF_LOG_LEVEL_TYPE_INFO, "Oaf_SetDefaultHeadset: serial_number = {}", serial_number);
  OafService_ActivateHeadset(serial_number, 2);
  if (_enable_steamvr_autostart) {
    OpenVRHelper::RunStartup();
  }
}

void rlvr::appfw::OafCallbacks::NotifyAirLinkPairingStart(const char* pairing_code, const char* serial_number) {}

uint8_t rlvr::appfw::OafCallbacks::ConnectIpcRemoteHeadsetClient() {
  //OAF_SERVICE_LOG("RLVR:Appfw", OAF_LOG_LEVEL_TYPE_INFO, "Oaf_ConnectIpcRemoteHeadsetClient: called.");
  return 1;
}
