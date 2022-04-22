cmake_minimum_required(VERSION 3.18)

project(StftPitchShift)

option(ENABLE_BUILTIN "Use included external sources (pocketfft)" ON)
option(BUILD_EXECUTABLE "Build executable" ON)
option(BUILD_EXAMPLES "Build examples" ON)

if (MSVC)

  # optionally treat warnings as errors
  # add_compile_options(/W3 /WX)

  # build with multiple processes
  add_compile_options(/MP)

else()

  # optionally treat warnings as errors
  # add_compile_options(-Wall -Werror)

endif()

include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")

if(BUILD_EXECUTABLE)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
endif()

if(BUILD_EXAMPLES)
  include("${CMAKE_CURRENT_LIST_DIR}/examples/example.cmake")
endif()
