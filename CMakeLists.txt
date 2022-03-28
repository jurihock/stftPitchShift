cmake_minimum_required(VERSION 3.19)

include("dr_libs/CMakeLists.txt")
include("pocketfft/CMakeLists.txt")
include("smb/CMakeLists.txt")

project(smbPitchShift)

add_executable(smbPitchShift main.cpp)
target_link_libraries(smbPitchShift dr_libs pocketfft smb)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT smbPitchShift)
