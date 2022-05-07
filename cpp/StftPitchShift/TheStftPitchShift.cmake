cmake_minimum_required(VERSION 3.1...3.18)

project(main)

include(GNUInstallDirs)

include("${CMAKE_CURRENT_LIST_DIR}/cxxopts/CMakeLists.txt")
include("${CMAKE_CURRENT_LIST_DIR}/dr_libs/CMakeLists.txt")

add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
  PROPERTIES OUTPUT_NAME "stftpitchshift"
)

target_sources(${PROJECT_NAME}
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/IO.h"
          "${CMAKE_CURRENT_LIST_DIR}/IO.cpp"
)

target_include_directories(${PROJECT_NAME}
  PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE stftpitchshift
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE cxxopts dr_libs
)

target_compile_features(${PROJECT_NAME}
  PRIVATE cxx_std_11
)

install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
