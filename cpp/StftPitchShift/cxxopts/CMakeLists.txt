add_library(cxxopts INTERFACE)

target_sources(cxxopts
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/cxxopts.hpp"
)

target_include_directories(cxxopts
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/.."
)
