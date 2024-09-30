#include "rlvr_util.h"

using namespace rlvr;

bool rlvr::Util::MaskCompare(void* buf, const char* pattern, const char* mask) {
  for (auto value = (uint8_t*)buf; *mask; ++pattern, ++mask, ++value) {
    if (*mask == 'x' && *(const uint8_t*)pattern != *value) return false;
  }
  return true;
}

uint8_t* rlvr::Util::FindPattern(void* base, size_t size, const char* pattern, const char* mask) {
  size -= strlen(mask);
  // TODO(Kaitlyn): Handle checking for whether the address is even accessible?
  for (size_t i = 0; i < size; ++i) {
    auto addr = (uint8_t*)base + i;
    if (MaskCompare(addr, pattern, mask)) return addr;
  }
  return nullptr;
}

uint8_t* rlvr::Util::FindPattern(HMODULE module_handle, const char* pattern, const char* mask) {
  MODULEINFO info{};
  GetModuleInformation(GetCurrentProcess(), module_handle, &info, sizeof(info));
  return FindPattern(info.lpBaseOfDll, info.SizeOfImage, pattern, mask);
}

std::string rlvr::Util::GetConcatPath_Utf8(const std::string& first, const std::string& second) {
  std::string temp = first;
  if (first[first.length()] != '\\') {
    temp += '\\';
    return temp + second;
  } else {
    return first + second;
  }
}

std::wstring rlvr::Util::GetConcatPath_Utf16(const std::wstring& first, const std::wstring& second) {
  std::wstring temp = first;
  if (first[first.length()] != '\\') {
    temp += '\\';
    return temp + second;
  } else {
    return first + second;
  }
}

std::wstring rlvr::Util::GetLocalAppDataDir() {
  PWSTR p_dir;
  SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &p_dir);
  return p_dir;
}
