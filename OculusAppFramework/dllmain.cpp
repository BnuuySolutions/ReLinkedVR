#include <windows.h>
#include <inttypes.h>
#include <stdio.h>

#if defined(_WIN32)
#define OVR_EXPORT extern "C" __declspec(dllexport)
#else
#error "Unsupported Platform."
#endif

#define OVRSERVER_UNK01_FUNCTION_ID 2
#define OVRSERVER_CHANGEFOCUS_FUNCTION_ID 3
#define OVRSERVER_CHANGEINPUTFOCUS_FUNCTION_ID 5
#define OVRSERVER_SETMODALSYSTEMOVERLAY_FUNCTION_ID 7
#define OVRSERVER_SETDEPTHBUFFERSREQUESTSTATUS_FUNCTION_ID 9
#define OVRSERVER_ACTIVATEHEADSET_FUNCTION_ID 24
#define OVRSERVER_SETUSEREYEHEIGHT_FUNCTION_ID 40
#define OVRSERVER_SETTRACKINGMODE_FUNCTION_ID 46
#define OVRSERVER_SETSYSTEMTRACKINGORIGIN_FUNCTION_ID 63
#define OVRSERVER_SETTRACKINGORIGIN_FUNCTION_ID 64

#define OAFSERVER_TESTCONNECTION_CALLBACK_ID 0
#define OAFSERVER_UPLOADCRASHREPORTS_CALLBACK_ID 1
#define OAFSERVER_RUN_CALLBACK_ID 2
#define OAFSERVER_TEARDOWN_CALLBACK_ID 3
#define OAF_NEWPROCESS_CALLBACK_ID 7
#define OAF_HMDEVENT_CALLBACK_ID 18
#define OAF_TRACKEREVENT_CALLBACK_ID 19
#define OAF_PLAYSPACEIS360_CALLBACK_ID 35

struct OvrVector3f {
  // rotation?
  float rotx;
  float roty;
  float rotz;
  float rotw;

  // position?
  float posx;
  float posy;
  float posz;
};

int32_t (*OVRServer_Unk01)() = nullptr;
int32_t (*OVRServer_ChangeFocus)(uint32_t) = nullptr;
int32_t (*OVRServer_ChangeInputFocus)(uint32_t, uint64_t) = nullptr;
int32_t (*OVRServer_SetModalSystemOverlay)(bool) = nullptr;
int32_t (*OVRServer_SetDepthBuffersRequestStatus)(bool) = nullptr;
int32_t (*OVRServer_ActivateHeadset)(const char[16], int32_t) = nullptr;
uint8_t (*OVRServer_SetUserEyeHeight)(double) = nullptr;
uint8_t (*OVRServer_SetTrackingMode)(bool) = nullptr;
uint8_t (*OVRServer_SetSystemTrackingOrigin)(OvrVector3f*) = nullptr;
uint8_t (*OVRServer_SetTrackingOrigin)(OvrVector3f*, int) = nullptr;

uint32_t Oaf_HMDEvent() {
  static const char* serialNumber = "WMHD3032200CBR";
  static char serialBlock[16];
  memset(serialBlock, 0, 16);
  memcpy_s(serialBlock, 16, serialNumber, strlen(serialNumber));
  static bool isActive = false;
  if (!isActive) {
    OVRServer_ActivateHeadset(serialBlock, 2);
    isActive = true;
  }
  return 0;
}

uint32_t Oaf_TrackerEvent() {
  static bool firstTime = false;
  if (!firstTime) {
    OVRServer_ChangeFocus(0);
    OVRServer_SetTrackingMode(false);
    OVRServer_ChangeInputFocus(0, 0);
    OVRServer_SetModalSystemOverlay(true);
    OVRServer_SetDepthBuffersRequestStatus(false);
    firstTime = true;
  }
  return 0;
}

uint32_t Oaf_NewProcess(uint32_t pid) {
  printf("turning tracking on\n");
  OVRServer_SetTrackingMode(false);
  OVRServer_ChangeFocus(pid);
  OVRServer_ChangeInputFocus(pid, 0);
  OVRServer_SetTrackingMode(true);
  //OVRServer_Unk01();
  // FIX ME!! DOES NOT TURN ON 6DOF
  OVRServer_SetUserEyeHeight(1.57); // hardcoded my height here (yes im 5'2"/157cm, please don't bully me)
  OvrVector3f vec{1,1,1,1,1};
  OVRServer_SetSystemTrackingOrigin(&vec);
  OVRServer_SetTrackingOrigin(&vec, 1);
  return 0;
}

uint32_t Oaf_PlayspaceIs360_threadsafe() {
  return 1;
}

OVR_EXPORT void OculusAppFrameworkLoad() {}
OVR_EXPORT bool OculusAppFrameworkInit(void* functionTable) {
  void** functions = reinterpret_cast<void**>(static_cast<char*>(functionTable) + sizeof(uint64_t));
  OVRServer_Unk01 = decltype(OVRServer_Unk01)(functions[OVRSERVER_UNK01_FUNCTION_ID]);
  OVRServer_ChangeFocus = decltype(OVRServer_ChangeFocus)(functions[OVRSERVER_CHANGEFOCUS_FUNCTION_ID]);
  OVRServer_ChangeInputFocus = decltype(OVRServer_ChangeInputFocus)(functions[OVRSERVER_CHANGEINPUTFOCUS_FUNCTION_ID]);
  OVRServer_SetModalSystemOverlay = decltype(OVRServer_SetModalSystemOverlay)(functions[OVRSERVER_SETMODALSYSTEMOVERLAY_FUNCTION_ID]);
  OVRServer_SetDepthBuffersRequestStatus = decltype(OVRServer_SetDepthBuffersRequestStatus)(functions[OVRSERVER_SETDEPTHBUFFERSREQUESTSTATUS_FUNCTION_ID]);
  OVRServer_ActivateHeadset = decltype(OVRServer_ActivateHeadset)(functions[OVRSERVER_ACTIVATEHEADSET_FUNCTION_ID]);
  OVRServer_SetUserEyeHeight = decltype(OVRServer_SetUserEyeHeight)(functions[OVRSERVER_SETUSEREYEHEIGHT_FUNCTION_ID]);
  OVRServer_SetTrackingMode = decltype(OVRServer_SetTrackingMode)(functions[OVRSERVER_SETTRACKINGMODE_FUNCTION_ID]);
  OVRServer_SetSystemTrackingOrigin = decltype(OVRServer_SetSystemTrackingOrigin)(functions[OVRSERVER_SETSYSTEMTRACKINGORIGIN_FUNCTION_ID]);
  OVRServer_SetTrackingOrigin = decltype(OVRServer_SetTrackingOrigin)(functions[OVRSERVER_SETTRACKINGORIGIN_FUNCTION_ID]);
  return false;
}

OVR_EXPORT void OculusAppFrameworkSetCallbacks(void* callbackTable) {
  void** callbacks = reinterpret_cast<void**>(static_cast<char*>(callbackTable) + sizeof(uint64_t)); // get to the actual callbacks, ignore table size.
  callbacks[OAF_HMDEVENT_CALLBACK_ID] = Oaf_HMDEvent;
  callbacks[OAF_TRACKEREVENT_CALLBACK_ID] = Oaf_TrackerEvent;
  callbacks[OAF_NEWPROCESS_CALLBACK_ID] = Oaf_NewProcess;
  callbacks[OAF_PLAYSPACEIS360_CALLBACK_ID] = Oaf_PlayspaceIs360_threadsafe;
}
