#pragma once

#include "rlvr_ovrtypes.h"
#include <atomic>

#define OVR_INTERFACE_IREFCOUNTED_IMPL \
  private: \
    std::atomic<int32_t> _ref_count = {1}; \
  public: \
    void AddRef() { \
      _ref_count.fetch_add(1, std::memory_order_relaxed); \
    } \
    void Release() { \
      if (_ref_count.fetch_add(-1, std::memory_order_relaxed) - 1 == 0) delete this; \
    }

namespace ovr_interface {

class IRefCounted {
public:
  virtual ~IRefCounted() = default;
  virtual void AddRef() = 0;
  virtual void Release() = 0;
  virtual void Destroy() { delete this; }
};
class IUnknown : public IRefCounted {
public:
  virtual void* QueryInterface(uint64_t iid) = 0;
};

}

namespace ovr {

static const uint64_t IID_IDirectDisplaySurface = 0xAD7152FE3248D8C4;
static const uint64_t IID_IDirectDisplay = 0xAD7152FE3248D8C6;
static const uint64_t IID_IDirectDisplayAPI = 0xAD7152FE3248D8C8;

class IDirectDisplaySurface : public ovr_interface::IUnknown {
public:
  virtual HANDLE GetSharedHandle() = 0;
};
class IDirectDisplay : public ovr_interface::IUnknown {
public:
  virtual ovrResult GetProperties(ovrDisplayProperties* out_properties) = 0;

  // No idea what this is, all of the IDirectDisplay implementations in OVRServer return unsupported.
  // So, we'll just follow the trend and return unsupported as well /shrug
  virtual ovrResult IDirectDisplay_Unk06() { return ovrError_Unsupported; }

  virtual ovrResult AcquireExclusive(::IUnknown* d3d_ptr) = 0;
  virtual ovrResult ReleaseExclusive() = 0;

  // TODO(Kaitlyn): No clue what these 2 functions do, stubbing these. Not important.
  virtual ovrResult GetCurrentMode(void* a1) { return ovrError_Unsupported; }
  virtual ovrResult SetMode(void* a1) { return ovrSuccess; }

  virtual ovrResult CreateSurface(ovrModeDesc* mode_desc, IDirectDisplaySurface** out_surface) = 0;
  virtual ovrResult CreateFrameNotification(void* a1, void* a2) { return ovrError_Unsupported; } // Not required, so we'll just return unsupported.

  virtual ovrResult Present(IDirectDisplaySurface* surface, void* a2) = 0; // Still don't know what "a2" is, can be ignored for now I think?

  // Basically the same as GetPresentState, except the WaitForSingleObject passes in the custom timeout.
  // There is also no "outState" variable, the ovrResult is the state of the present state.
  // For example: ovrResult = ovrSuccess if the WaitForSingleObject is non-signaled.
  // Whatever else if there is a specific error when waiting for object.
  virtual ovrResult GetPresentStateTimeout(int32_t timeout_ms) = 0;
  virtual ovrResult GetPresentStats(ovrPresentStats* out_present_stats) = 0;

  // TODO(Kaitlyn): These 2 functions have something to do with display visibility, no idea what they do.
  // Never called in practice, as far as I know... We'll just stub them, shouldn't matter.
  virtual ovrResult IDirectDisplay_Unk16(void* a1) { return ovrSuccess; }
  virtual ovrResult IDirectDisplay_Unk17() { return ovrSuccess; }

  virtual ovrResult GetPresentState(bool* out_state) = 0; // outValue: True = Present signaled (hangs rendering), False = Present non-signaled
  virtual ovrResult GetHdcpState(bool* out_state) = 0; // outValue: True = HDCP active, False = HDCP inactive
};
class IDirectDisplayAPI : public ovr_interface::IUnknown {
public:
  virtual ovrResult EnableDirectDisplay(bool value) = 0;

  // TODO(Kaitlyn): Probably used for hotplugging, we'll stub this for now. Will come back to this later, hopefully.
  virtual ovrResult RegisterCallbacks(void* a1, void* a2) { return ovrSuccess; }

  // Used for getting displays by index? Never called in practice, stubbed as a result.
  virtual ovrResult IDirectDisplayAPI_Unk07(void* a1) { return ovrSuccess; }

  virtual ovrResult Enumerate(uint32_t index, IDirectDisplay** out_direct_display) = 0;
  virtual ovrResult GetName(char* out_name) = 0;
};

}
