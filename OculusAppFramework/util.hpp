#pragma once
#include <windows.h>
#include <shlobj.h>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class util {
public:
  static std::string GetConcatPath_Utf8(const std::string& first, const std::string& second) {
    std::string temp = first;
    if (first[first.length()] != '\\') {
      temp += '\\';
      return temp + second;
    } else {
      return first + second;
    }
  }

  static std::wstring GetConcatPath_Utf16(const std::wstring& first, const std::wstring& second) {
    std::wstring temp = first;
    if (first[first.length()] != '\\') {
      temp += '\\';
      return temp + second;
    } else {
      return first + second;
    }
  }

  static std::wstring GetLocalAppDataDir() {
    PWSTR p_dir;
    SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &p_dir);
    return p_dir;
  }

  static bool FileExists(LPCWSTR lpPath) {
    DWORD attrib = GetFileAttributesW(lpPath);
    return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
  }

  static json GetJSONFile(const std::wstring& filePath) {
    std::ifstream f(filePath);
    return json::parse(f);
  }
};
