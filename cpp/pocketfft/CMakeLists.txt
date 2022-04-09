add_library(pocketfft INTERFACE
  "${CMAKE_CURRENT_LIST_DIR}/pocketfft_hdronly.h"
)

target_include_directories(pocketfft
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/.."
)
