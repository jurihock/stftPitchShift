cmake_minimum_required(VERSION 3.19)

project(smbPitchShift)

add_executable(smbPitchShift main.cpp dr_wav.h smbPitchShift.h smbPitchShift.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT smbPitchShift)
