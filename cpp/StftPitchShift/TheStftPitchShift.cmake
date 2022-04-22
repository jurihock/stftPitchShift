cmake_minimum_required(VERSION 3.18)

project(TheStftPitchShift)

include("${CMAKE_CURRENT_LIST_DIR}/anyoption/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/dr_libs/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/smb/CMakeLists.txt")

add_executable(TheStftPitchShift)

set_target_properties(TheStftPitchShift
  PROPERTIES OUTPUT_NAME "stftpitchshift"
)

target_sources(TheStftPitchShift
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/IO.h"
          "${CMAKE_CURRENT_LIST_DIR}/IO.cpp"
)

target_include_directories(TheStftPitchShift
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_link_libraries(TheStftPitchShift
  PRIVATE LibStftPitchShift
)

target_link_libraries(TheStftPitchShift
  PRIVATE anyoption dr_libs smb
)

target_compile_features(TheStftPitchShift
  PRIVATE cxx_std_11
)
