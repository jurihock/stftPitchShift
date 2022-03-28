cmake_minimum_required(VERSION 3.19)

include("anyoption/CMakeLists.txt")
include("dr_libs/CMakeLists.txt")
include("pocketfft/CMakeLists.txt")
include("smb/CMakeLists.txt")

project(smbPitchShift)

add_executable(smbPitchShift
  main.cpp
  IO.h IO.cpp
  STFT.h STFT.cpp
  Vocoder.h Vocoder.cpp
)

target_link_libraries(smbPitchShift
  anyoption dr_libs pocketfft smb
)

target_include_directories(smbPitchShift
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}"
)

set_property(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTY VS_STARTUP_PROJECT smbPitchShift
)
