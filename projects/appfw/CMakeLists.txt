cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(appfw)

include_directories("include")
include_directories("${ovrcore_SOURCE_DIR}/include")
include_directories("${websocketpp_SOURCE_DIR}")
#include_directories("${asio_SOURCE_DIR}/asio/include")

add_library(appfw SHARED
  "src/main.cpp"
)

#add_definitions(-DASIO_STANDALONE)
target_link_libraries(appfw PRIVATE nlohmann_json)
#if(WIN32)
#  target_link_libraries(appfw PRIVATE wsock32 ws2_32)
#endif()
set_target_properties(appfw PROPERTIES OUTPUT_NAME "OculusAppFramework")
