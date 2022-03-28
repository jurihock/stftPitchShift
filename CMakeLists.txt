cmake_minimum_required(VERSION 3.19)

include("dr_libs/CMakeLists.txt")
include("pocketfft/CMakeLists.txt")

project(smbPitchShift)

add_executable(smbPitchShift main.cpp smbPitchShift.h smbPitchShift.cpp)
target_link_libraries(smbPitchShift dr_libs pocketfft)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT smbPitchShift)
