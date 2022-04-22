cmake_minimum_required(VERSION 3.18)

project(StftPitchShift)

option(VCPKG "Enable vcpkg library only build excluding executables" OFF)

if (MSVC)

  # optionally treat warnings as errors
  # add_compile_options(/W3 /WX)

  # build with multiple processes
  add_compile_options(/MP)

else()

  # optionally treat warnings as errors
  # add_compile_options(-Wall -Werror)

endif()

if(VCPKG)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShiftVcpkg.cmake")
else()
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/examples/example.cmake")
endif()
