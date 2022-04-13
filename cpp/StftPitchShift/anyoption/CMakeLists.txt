add_library(anyoption)

target_sources(anyoption
  PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/anyoption.h"
          "${CMAKE_CURRENT_LIST_DIR}/anyoption.cpp"
)

target_include_directories(anyoption
  PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/.."
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)
