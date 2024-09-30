#include <rlvr_oaf.h>
#include <rlvr_ovrtypes.h>
#include "oaf_callbacks.h"

using namespace rlvr;
using namespace rlvr::appfw;
using namespace rlvr::oaf_service;

OVR_EXPORT void OculusAppFrameworkLoad() {}
OVR_EXPORT bool OculusAppFrameworkInit(void* functionTable) {
  void** functions = reinterpret_cast<void**>(static_cast<char*>(functionTable) + sizeof(uint64_t));
  OafService_Init(functions);
  return false;
}
OVR_EXPORT void OculusAppFrameworkSetCallbacks(void* callbackTable) {
  void** callbacks = reinterpret_cast<void**>(static_cast<char*>(callbackTable) + sizeof(uint64_t));
  IOafCallbacks::oaf_callbacks = new OafCallbacks;
  IOafCallbacks::Init(callbacks);
}
