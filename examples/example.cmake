cmake_minimum_required(VERSION 3.19)

project(example)

# don't forget to include LibStftPitchShift.cmake

add_executable(example "${CMAKE_CURRENT_LIST_DIR}/example.cpp")
target_link_libraries(example LibStftPitchShift)
target_compile_features(example PRIVATE cxx_std_11)
