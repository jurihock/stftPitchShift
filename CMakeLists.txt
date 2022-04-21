cmake_minimum_required(VERSION 3.18)

project(StftPitchShift)

# set this to OFF for library-only build
option(ENABLE_IO "Use dr_libs WAV I/O (needed for executable)" ON)
option(BUILD_EXAMPLES "build examples" ON)

if (MSVC)
  # add_compile_options(/W3 /WX)
  add_compile_options(/MP)
else()
  # add_compile_options(-Wall -Werror)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")

if(ENABLE_IO)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
endif()

if(BUILD_EXAMPLES)
  include("${CMAKE_CURRENT_LIST_DIR}/examples/example.cmake")
endif()
