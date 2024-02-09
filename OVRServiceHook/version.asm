.code
extern GetFileVersionInfoA_o:QWORD
extern GetFileVersionInfoByHandle_o:QWORD
extern GetFileVersionInfoExA_o:QWORD
extern GetFileVersionInfoExW_o:QWORD
extern GetFileVersionInfoSizeA_o:QWORD
extern GetFileVersionInfoSizeExA_o:QWORD
extern GetFileVersionInfoSizeExW_o:QWORD
extern GetFileVersionInfoSizeW_o:QWORD
extern GetFileVersionInfoW_o:QWORD
extern VerFindFileA_o:QWORD
extern VerFindFileW_o:QWORD
extern VerInstallFileA_o:QWORD
extern VerInstallFileW_o:QWORD
extern VerLanguageNameA_o:QWORD
extern VerLanguageNameW_o:QWORD
extern VerQueryValueA_o:QWORD
extern VerQueryValueW_o:QWORD
GetFileVersionInfoA_OVR proc
  jmp QWORD ptr GetFileVersionInfoA_o
GetFileVersionInfoA_OVR endp
GetFileVersionInfoByHandle_OVR proc
  jmp QWORD ptr GetFileVersionInfoByHandle_o
GetFileVersionInfoByHandle_OVR endp
GetFileVersionInfoExA_OVR proc
  jmp QWORD ptr GetFileVersionInfoExA_o
GetFileVersionInfoExA_OVR endp
GetFileVersionInfoExW_OVR proc
  jmp QWORD ptr GetFileVersionInfoExW_o
GetFileVersionInfoExW_OVR endp
GetFileVersionInfoSizeA_OVR proc
  jmp QWORD ptr GetFileVersionInfoSizeA_o
GetFileVersionInfoSizeA_OVR endp
GetFileVersionInfoSizeExA_OVR proc
  jmp QWORD ptr GetFileVersionInfoSizeExA_o
GetFileVersionInfoSizeExA_OVR endp
GetFileVersionInfoSizeExW_OVR proc
  jmp QWORD ptr GetFileVersionInfoSizeExW_o
GetFileVersionInfoSizeExW_OVR endp
GetFileVersionInfoSizeW_OVR proc
  jmp QWORD ptr GetFileVersionInfoSizeW_o
GetFileVersionInfoSizeW_OVR endp
GetFileVersionInfoW_OVR proc
  jmp QWORD ptr GetFileVersionInfoW_o
GetFileVersionInfoW_OVR endp
VerFindFileA_OVR proc
  jmp QWORD ptr VerFindFileA_o
VerFindFileA_OVR endp
VerFindFileW_OVR proc
  jmp QWORD ptr VerFindFileW_o
VerFindFileW_OVR endp
VerInstallFileA_OVR proc
  jmp QWORD ptr VerInstallFileA_o
VerInstallFileA_OVR endp
VerInstallFileW_OVR proc
  jmp QWORD ptr VerInstallFileW_o
VerInstallFileW_OVR endp
VerLanguageNameA_OVR proc
  jmp QWORD ptr VerLanguageNameA_o
VerLanguageNameA_OVR endp
VerLanguageNameW_OVR proc
  jmp QWORD ptr VerLanguageNameW_o
VerLanguageNameW_OVR endp
VerQueryValueA_OVR proc
  jmp QWORD ptr VerQueryValueA_o
VerQueryValueA_OVR endp
VerQueryValueW_OVR proc
  jmp QWORD ptr VerQueryValueW_o
VerQueryValueW_OVR endp
end
