cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(appfw)

include_directories("include")
include_directories("${ovrcore_SOURCE_DIR}/include")
include_directories("${websocketpp_SOURCE_DIR}")

add_library(appfw SHARED
  "src/main.cpp"
)

target_link_libraries(appfw PRIVATE nlohmann_json)
set_target_properties(appfw PROPERTIES OUTPUT_NAME "OculusAppFramework")
