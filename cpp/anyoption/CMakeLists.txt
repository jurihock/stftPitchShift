add_library(anyoption
  "${CMAKE_CURRENT_LIST_DIR}/anyoption.h"
  "${CMAKE_CURRENT_LIST_DIR}/anyoption.cpp"
)

target_include_directories(anyoption
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)
