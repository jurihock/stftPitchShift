cmake_minimum_required(VERSION 3.19)

project(LibStftPitchShift)

include("${CMAKE_CURRENT_LIST_DIR}/dr_libs/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/pocketfft/CMakeLists.txt")

add_library(LibStftPitchShift
  "${CMAKE_CURRENT_LIST_DIR}/Cepstrum.h"
  "${CMAKE_CURRENT_LIST_DIR}/Cepstrum.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/IO.h"
  "${CMAKE_CURRENT_LIST_DIR}/IO.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Pitcher.h"
  "${CMAKE_CURRENT_LIST_DIR}/Pitcher.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Resampler.h"
  "${CMAKE_CURRENT_LIST_DIR}/Resampler.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/STFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/STFT.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.h"
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/Timer.h"
  "${CMAKE_CURRENT_LIST_DIR}/Vocoder.h"
  "${CMAKE_CURRENT_LIST_DIR}/Vocoder.cpp"
)

set_target_properties(LibStftPitchShift
  PROPERTIES OUTPUT_NAME "StftPitchShift"
)

target_include_directories(LibStftPitchShift
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_link_libraries(LibStftPitchShift
  dr_libs pocketfft
)

target_compile_features(LibStftPitchShift
  PRIVATE cxx_std_11
)
