cmake_minimum_required(VERSION 3.1...3.18)

project(StftPitchShiftExamples)

# basic example of single file processing
add_executable(example "${CMAKE_CURRENT_LIST_DIR}/example.cpp")

# advanced example of continuous stream processing
add_executable(realtime "${CMAKE_CURRENT_LIST_DIR}/realtime.cpp")

foreach(name example realtime)

  # at least C++ 20 is mandatory
  target_compile_features(${name} PRIVATE cxx_std_20)

  # just the case of default build e.g. if LibStftPitchShift.cmake is already included
  target_link_libraries(${name} stftpitchshift)

  # if installed via ppa:jurihock/stftpitchshift on Ubuntu
  # target_link_libraries(${name} stftpitchshift)

  # if installed via vcpkg
  # find_package(stftpitchshift CONFIG REQUIRED)
  # target_link_libraries(${name} stftpitchshift::stftpitchshift)

endforeach()
