cmake_minimum_required(VERSION 3.1...3.18)

project(StftPitchShift)

option(SHARED "Enable shared library build" OFF)
option(VCPKG  "Enable vcpkg library only build excluding executable" OFF)
option(DEB    "Enable deb package build for library and executable" OFF)

if (MSVC)

  # optionally treat warnings as errors
  # add_compile_options(/W3 /WX)

  # build with multiple processes
  add_compile_options(/MP)

else()

  # optionally treat warnings as errors
  # add_compile_options(-Wall -Werror)

endif()

if(VCPKG AND DEB)
  message(FATAL_ERROR "Please enable only one option VCPKG or DEB, but not both at once!")
endif()

if(VCPKG)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")
elseif(DEB)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/DebStftPitchShift.cmake")
else()
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/examples/example.cmake")
endif()
