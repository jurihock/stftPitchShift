cmake_minimum_required(VERSION 3.1...3.18)

project(StftPitchShiftProject)

option(VCPKG "Enable vcpkg compatible library only build excluding executable" OFF)
option(DEB   "Enable deb package build for library and executable" OFF)
option(BREW  "Enable homebrew package build for library and executable" OFF)

if(MSVC)

  # optionally treat warnings as errors
  # add_compile_options(/W3 /WX)

  # build with multiple processes
  add_compile_options(/MP)

else()

  # optionally treat warnings as errors
  # add_compile_options(-Wall -Werror)

endif()

if(MSVC AND BUILD_SHARED_LIBS)

  # automatically create module definition files on Windows
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

endif()

if((VCPKG AND DEB) OR (DEB AND BREW) OR (VCPKG AND BREW))
  message(FATAL_ERROR "Please enable only one option {VCPKG,DEB,BREW}, but not multiple at once!")
endif()

if(VCPKG)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/VcpkgStftPitchShift.cmake")
elseif(DEB)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/DebStftPitchShift.cmake")
elseif(BREW)
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/BrewStftPitchShift.cmake")
else()
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/LibStftPitchShift.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/cpp/StftPitchShift/TheStftPitchShift.cmake")
  include("${CMAKE_CURRENT_LIST_DIR}/examples/CMakeLists.txt")
endif()
