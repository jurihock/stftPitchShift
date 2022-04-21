cmake_minimum_required(VERSION 3.18)

project(StftPitchShift)

option(ENABLE_BUILTIN "Use included external sources (drlibs wav I/O, pocketfft)" ON)
option(ENABLE_IO "enable wav I/O support (needed for executable)" ON)
option(BUILD_EXAMPLES "build examples" ON)
option(BUILD_EXECUTABLE "build executable" ON)

if (MSVC)
  # add_compile_options(/W3 /WX)
  add_compile_options(/MP)
else()
  # add_compile_options(-Wall -Werror)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")

if(BUILD_EXECUTABLE)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
endif()

if(BUILD_EXAMPLES)
  include("${CMAKE_CURRENT_LIST_DIR}/examples/example.cmake")
endif()
