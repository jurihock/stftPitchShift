cmake_minimum_required(VERSION 3.19)

project(StftPitchShift)

if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Werror)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/examples/example.cmake")
