#pragma once
#include <atomic>

#define OVR_INTERFACE_IREFCOUNTED_IMPL \
  private: \
    std::atomic<int> RefCount = {1}; \
  public: \
    void AddRef() { \
      RefCount.fetch_add(1, std::memory_order_relaxed); \
    } \
    void Release() { \
      if (RefCount.fetch_add(-1, std::memory_order_relaxed) - 1 == 0) { \
        delete this; \
      } \
    }

namespace OVRInterface {

class IRefCounted {
public:
  virtual ~IRefCounted() = default;

  virtual void AddRef() = 0;
  virtual void Release() = 0;

  // This isn't apart of standard COM, no idea what Oculus was thinking.
  // This also defeats the purpose of an interface, but whatever.
  virtual void Destroy() { delete this; }
};

// Oculus uses this abhorrent COM-like interface structure, pain.
class IUnknown : public IRefCounted {
public:
  virtual void* QueryInterface(uint64_t iid) = 0;
};

class IInterfaceFactory : public IUnknown {
public:
};

class IAggregateInterfaceFactory : public IInterfaceFactory {
public:
};

class OVRAggregateInterfaceFactory : public IAggregateInterfaceFactory {
public:
};

class OVRInterfaceManager : public OVRAggregateInterfaceFactory {
public:
};

} // OVRInterface
