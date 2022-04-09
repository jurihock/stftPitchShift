cmake_minimum_required(VERSION 3.19)

project(TheStftPitchShift)

include("anyoption/CMakeLists.txt")
include("dr_libs/CMakeLists.txt")
include("smb/CMakeLists.txt")

add_executable(TheStftPitchShift
  main.cpp
  IO.h IO.cpp
)

set_target_properties(TheStftPitchShift
  PROPERTIES OUTPUT_NAME "StftPitchShift"
)

target_include_directories(TheStftPitchShift
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_link_libraries(TheStftPitchShift
  LibStftPitchShift
)

target_link_libraries(TheStftPitchShift
  anyoption dr_libs smb
)

target_compile_features(TheStftPitchShift
  PRIVATE cxx_std_11
)
