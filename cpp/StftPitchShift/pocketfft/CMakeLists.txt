add_library(pocketfft INTERFACE)

target_sources(pocketfft
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/pocketfft_hdronly.h"
)

target_include_directories(pocketfft
  INTERFACE "${CMAKE_CURRENT_LIST_DIR}/.."
)

if (UNIX)
  target_link_libraries(pocketfft
    INTERFACE pthread)
endif()
