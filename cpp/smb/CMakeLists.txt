add_library(smb
  "${CMAKE_CURRENT_LIST_DIR}/smbPitchShift.h"
  "${CMAKE_CURRENT_LIST_DIR}/smbPitchShift.cpp"
)

target_include_directories(smb
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
)
