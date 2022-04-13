cmake_minimum_required(VERSION 3.18)

project(LibStftPitchShift)

include("${CMAKE_CURRENT_LIST_DIR}/dr_libs/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/pocketfft/CMakeLists.txt")

add_library(LibStftPitchShift)

set_target_properties(LibStftPitchShift
  PROPERTIES OUTPUT_NAME "stftpitchshift"
)

target_sources(LibStftPitchShift
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Cepstrum.h"
         "${CMAKE_CURRENT_LIST_DIR}/Cepstrum.cpp"
         "${CMAKE_CURRENT_LIST_DIR}/Dump.h"
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
         "${CMAKE_CURRENT_LIST_DIR}/Version.h"
         "${CMAKE_CURRENT_LIST_DIR}/Vocoder.h"
         "${CMAKE_CURRENT_LIST_DIR}/Vocoder.cpp"
)

target_include_directories(LibStftPitchShift
  PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/.."
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

target_link_libraries(LibStftPitchShift
  PUBLIC dr_libs pocketfft
)

target_compile_features(LibStftPitchShift
  PRIVATE cxx_std_11
)
