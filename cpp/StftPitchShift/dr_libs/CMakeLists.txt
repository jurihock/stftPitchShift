add_library(dr_libs INTERFACE)

target_sources(dr_libs
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/dr_wav.h"
)

target_include_directories(dr_libs
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/.."
)
