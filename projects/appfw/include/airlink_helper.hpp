#pragma once
#include <stdio.h>
#include <windows.h>
#include <ovr_oaf.hpp>

#define TEXT_SIZE 255

class airlink_helper {
private:
  struct PairingThreadData {
    char* pairingCode;
    char* serialNumber;
  };

  static DWORD WINAPI PairingThread(LPVOID lpParam) {
    auto threadData = reinterpret_cast<PairingThreadData*>(lpParam);
    char text[TEXT_SIZE];
    snprintf(text, TEXT_SIZE, "Your Air Link pairing code is: %s\nPlease confirm this in your headset before continuing.", threadData->pairingCode);
    int result = MessageBoxA(nullptr, text, "Air Link Pairing", MB_OKCANCEL);
    Oaf_OVRService_SetAirLinkPairingResult(result == IDOK ? true : false, threadData->serialNumber);
    return 0;
  }

public:
  static void StartPairing(const char* pairingCode, const char* serialNumber) {
    auto threadData = new PairingThreadData;
    threadData->pairingCode = reinterpret_cast<char*>(malloc(strlen(pairingCode)));
    if (threadData->pairingCode) {
      memcpy(threadData->pairingCode, pairingCode, strlen(pairingCode));
    }
    threadData->serialNumber = reinterpret_cast<char*>(malloc(strlen(serialNumber)));
    if (threadData->serialNumber) {
      memcpy(threadData->serialNumber, serialNumber, strlen(serialNumber));
    }
    CreateThread(0, 0, PairingThread, reinterpret_cast<LPVOID>(threadData), 0, 0);
  }
};
