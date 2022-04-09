cmake_minimum_required(VERSION 3.19)

project(StftPitchShift)

include("LibStftPitchShift.cmake")
include("TheStftPitchShift.cmake")

set_property(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTY VS_STARTUP_PROJECT TheStftPitchShift
)
