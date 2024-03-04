cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(svchook)

enable_language(C ASM_MASM)

include_directories("proxylib/include")

add_library(svchook SHARED
  "proxylib/version.def"
  "proxylib/version.asm"
  "src/main.cpp"
)

target_link_libraries(svchook PRIVATE minhook)
set_target_properties(svchook PROPERTIES OUTPUT_NAME "version")
