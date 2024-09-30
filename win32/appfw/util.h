#pragma once

#include <windows.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace rlvr {
namespace appfw {

class Util {
public:
  static bool IsProcessOpen(HANDLE process_handle) {
    DWORD exit_code;
    if (GetExitCodeProcess(process_handle, &exit_code) && exit_code == STILL_ACTIVE) {
      return true;
    }
    return false;
  }

  static json GetJSONFile(const std::wstring& file_name) {
    std::ifstream f(file_name);
    return json::parse(f);
  }
};

}
}
