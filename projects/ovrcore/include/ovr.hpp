#pragma once
#include <cstdint>
#if defined(_WIN32)
#define OVR_EXPORT extern "C" __declspec(dllexport)
#else
#error "Unsupported Platform."
#endif

typedef char ovrBool;
#define ovrFalse 0
#define ovrTrue 1

typedef int32_t ovrResult;

typedef enum ovrSuccessType_ {
  ovrSuccess = 0
} ovrSuccessType;

typedef enum ovrErrorType_ {
  ovrError_InvalidParameter = -1005,
  ovrError_Unsupported = -1009
} ovrErrorType;

typedef enum ovrTextureFormat_ {
  OVR_FORMAT_UNKNOWN = 0,
  OVR_FORMAT_B5G6R5_UNORM = 1,
  OVR_FORMAT_B5G5R5A1_UNORM = 2,
  OVR_FORMAT_B4G4R4A4_UNORM = 3,
  OVR_FORMAT_R8G8B8A8_UNORM = 4,
  OVR_FORMAT_R8G8B8A8_UNORM_SRGB = 5,
  OVR_FORMAT_B8G8R8A8_UNORM = 6,
  OVR_FORMAT_B8G8R8_UNORM = 27, // Not supported.
  OVR_FORMAT_B8G8R8A8_UNORM_SRGB = 7,
  OVR_FORMAT_B8G8R8X8_UNORM = 8,
  OVR_FORMAT_B8G8R8X8_UNORM_SRGB = 9,
  OVR_FORMAT_R16G16B16A16_FLOAT = 10,
  OVR_FORMAT_R11G11B10_FLOAT = 25,
  OVR_FORMAT_R32_FLOAT = 26,
  OVR_FORMAT_D16_UNORM = 11,
  OVR_FORMAT_D24_UNORM_S8_UINT = 12,
  OVR_FORMAT_D32_FLOAT = 13,
  OVR_FORMAT_D32_FLOAT_S8X24_UINT = 14,
  OVR_FORMAT_BC1_UNORM = 15,
  OVR_FORMAT_BC1_UNORM_SRGB = 16,
  OVR_FORMAT_BC2_UNORM = 17,
  OVR_FORMAT_BC2_UNORM_SRGB = 18,
  OVR_FORMAT_BC3_UNORM = 19,
  OVR_FORMAT_BC3_UNORM_SRGB = 20,
  OVR_FORMAT_BC6H_UF16 = 21,
  OVR_FORMAT_BC6H_SF16 = 22,
  OVR_FORMAT_BC7_UNORM = 23,
  OVR_FORMAT_BC7_UNORM_SRGB = 24,
  OVR_FORMAT_LAST,
  OVR_FORMAT_ENUMSIZE = 0x7fffffff
} ovrTextureFormat;
