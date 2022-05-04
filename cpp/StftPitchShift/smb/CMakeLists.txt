add_library(smb STATIC)

target_sources(smb
  PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/smbPitchShift.h"
          "${CMAKE_CURRENT_LIST_DIR}/smbPitchShift.cpp"
)

target_include_directories(smb
  PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/.."
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)
