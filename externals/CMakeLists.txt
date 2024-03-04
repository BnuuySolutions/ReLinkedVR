add_subdirectory(websocketpp)
add_subdirectory(json)

# Win32-only externals
if(WIN32)
  add_subdirectory(minhook)
endif()
