#pragma once
#if defined(_WIN32)
#include <dxgi.h>
typedef DXGI_RATIONAL ovrRational;
#else
#error "Unsupported Platform."
#endif

typedef struct ovrSizei_ {
  int w, h;
} ovrSizei;
