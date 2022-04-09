cmake_minimum_required(VERSION 3.19)

include("anyoption/CMakeLists.txt")
include("dr_libs/CMakeLists.txt")
include("pocketfft/CMakeLists.txt")
include("smb/CMakeLists.txt")

project(smbPitchShift)

add_executable(smbPitchShift
  main.cpp
  Cepstrum.h Cepstrum.cpp
  IO.h IO.cpp
  Pitcher.h Pitcher.cpp
  Resampler.h Resampler.cpp
  STFT.h STFT.cpp
  Timer.h
  Vocoder.h Vocoder.cpp
)

target_link_libraries(smbPitchShift
  anyoption dr_libs pocketfft smb
)

target_include_directories(smbPitchShift
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}"
)

target_compile_features(smbPitchShift
  PRIVATE cxx_std_11
)

set_property(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTY VS_STARTUP_PROJECT smbPitchShift
)
