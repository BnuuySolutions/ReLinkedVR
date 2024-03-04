#pragma once
#include "OVR.hpp"

// OVRService function IDs
#define OAF_OVRSERVICE_CHANGEFOCUS_FUNCTION_ID 3
#define OAF_OVRSERVICE_CHANGEINPUTFOCUS_FUNCTION_ID 5
#define OAF_OVRSERVICE_SETMODALSYSTEMOVERLAY_FUNCTION_ID 7
#define OAF_OVRSERVICE_SETDEPTHBUFFERSREQUESTSTATUS_FUNCTION_ID 9
#define OAF_OVRSERVICE_ACTIVATEHEADSET_FUNCTION_ID 24
#define OAF_OVRSERVICE_SETTRACKINGMODE_FUNCTION_ID 46
#define OAF_OVRSERVICE_SETAIRLINKPAIRINGRESULT_FUNCTION_ID 86

// OAF callback IDs
#define OAFSERVER_TESTCONNECTION_CALLBACK_ID 0
#define OAFSERVER_UPLOADCRASHREPORTS_CALLBACK_ID 1
#define OAFSERVER_RUN_CALLBACK_ID 2
#define OAFSERVER_TEARDOWN_CALLBACK_ID 3
#define OAF_NEWPROCESS_CALLBACK_ID 7
#define OAF_INPUTEVENT_CALLBACK_ID 16
#define OAF_HMDEVENT_CALLBACK_ID 18
#define OAF_TRACKEREVENT_CALLBACK_ID 19
#define OAF_SETDEFAULTHEADSET_CALLBACK_ID 59
#define OAF_NOTIFYAIRLINKPAIRINGSTART_CALLBACK_ID 70

typedef enum oafInputEventType_ {
  // NOTE(Kaitlyn): Is this correct?
  // This event also has a sub-category of events, seehere: oafInputControllerEventType
  // I'm unaware as to if this is the correct way of doing this, but oh well. We'll find out.
  OAF_INPUT_EVENT_TYPE_CONTROLLER = 1,
  OAF_INPUT_EVENT_TYPE_PROXIMITY_SENSOR = 3,
  OAF_INPUT_EVENT_TYPE_DEVICE_MOTION = 4
} oafInputEventType;

// Is this also even correct?
typedef enum oafInputControllerEventType_ {
  OAF_INPUT_CONTROLLER_EVENT_TYPE_OCULUS_BUTTON = 1
} oafInputControllerEventType;

typedef enum oafHMDEventType_ {
  OAF_HMD_EVENT_TYPE_HMD_ATTACHED = 1, // Only seems to be fired after HMDs and Displays are connected fully.
  OAF_HMD_EVENT_TYPE_IAD_CHANGED = 9 // Fired when IPD is changed.
} oafHMDEventType;

// OVRService functions
// TODO(Kaitlyn): Determine return values, if at all, also figure out unknown parameters as well.
static int32_t (*Oaf_OVRService_ChangeFocus)(uint32_t processId) = nullptr;
static int32_t (*Oaf_OVRService_ChangeInputFocus)(uint32_t processId, uint64_t a2) = nullptr;
static int32_t (*Oaf_OVRService_SetModalSystemOverlay)(ovrBool value) = nullptr;
static int32_t (*Oaf_OVRService_SetDepthBuffersRequestStatus)(ovrBool value) = nullptr;
static int32_t (*Oaf_OVRService_ActivateHeadset)(const char serialNumber[16], int32_t a2) = nullptr;
static uint8_t (*Oaf_OVRService_SetTrackingMode)(ovrBool value) = nullptr;
static uint8_t (*Oaf_OVRService_SetAirLinkPairingResult)(ovrBool value, const char serialNumber[16]) = nullptr;

static void OculusAppFrameworkInitFunctions(void** functions) {
  Oaf_OVRService_ChangeFocus = decltype(Oaf_OVRService_ChangeFocus)(functions[OAF_OVRSERVICE_CHANGEFOCUS_FUNCTION_ID]);
  Oaf_OVRService_ChangeInputFocus = decltype(Oaf_OVRService_ChangeInputFocus)(functions[OAF_OVRSERVICE_CHANGEINPUTFOCUS_FUNCTION_ID]);
  Oaf_OVRService_SetModalSystemOverlay = decltype(Oaf_OVRService_SetModalSystemOverlay)(functions[OAF_OVRSERVICE_SETMODALSYSTEMOVERLAY_FUNCTION_ID]);
  Oaf_OVRService_SetDepthBuffersRequestStatus = decltype(Oaf_OVRService_SetDepthBuffersRequestStatus)(functions[OAF_OVRSERVICE_SETDEPTHBUFFERSREQUESTSTATUS_FUNCTION_ID]);
  Oaf_OVRService_ActivateHeadset = decltype(Oaf_OVRService_ActivateHeadset)(functions[OAF_OVRSERVICE_ACTIVATEHEADSET_FUNCTION_ID]);
  Oaf_OVRService_SetTrackingMode = decltype(Oaf_OVRService_SetTrackingMode)(functions[OAF_OVRSERVICE_SETTRACKINGMODE_FUNCTION_ID]);
  Oaf_OVRService_SetAirLinkPairingResult = decltype(Oaf_OVRService_SetAirLinkPairingResult)(functions[OAF_OVRSERVICE_SETAIRLINKPAIRINGRESULT_FUNCTION_ID]);
}
