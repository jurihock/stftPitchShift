cmake_minimum_required(VERSION 3.1...3.18)

project(TheStftPitchShift)

include(GNUInstallDirs)

include("${CMAKE_CURRENT_LIST_DIR}/cxxopts/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/dr_libs/CMakeLists.txt")

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
  PRIVATE cxxopts dr_libs
)

target_compile_features(TheStftPitchShift
  PRIVATE cxx_std_11
)

install(TARGETS TheStftPitchShift
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
