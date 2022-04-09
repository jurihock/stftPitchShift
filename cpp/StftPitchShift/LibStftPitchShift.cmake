cmake_minimum_required(VERSION 3.19)

project(LibStftPitchShift)

include("pocketfft/CMakeLists.txt")

add_library(LibStftPitchShift
  Cepstrum.h Cepstrum.cpp
  Pitcher.h Pitcher.cpp
  Resampler.h Resampler.cpp
  STFT.h STFT.cpp
  StftPitchShift.h StftPitchShift.cpp
  Timer.h
  Vocoder.h Vocoder.cpp
)

set_target_properties(LibStftPitchShift
  PROPERTIES OUTPUT_NAME "StftPitchShift"
)

target_include_directories(LibStftPitchShift
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)

target_link_libraries(LibStftPitchShift
  pocketfft
)

target_compile_features(LibStftPitchShift
  PRIVATE cxx_std_11
)
