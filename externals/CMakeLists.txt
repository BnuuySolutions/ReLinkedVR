set(imgui_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui" CACHE INTERNAL "")

add_subdirectory("websocketpp")
add_subdirectory("json")
add_subdirectory("SDL")

# Win32-only externals
if(WIN32)
  add_subdirectory("minhook")
endif()
