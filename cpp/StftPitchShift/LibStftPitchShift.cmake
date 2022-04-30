cmake_minimum_required(VERSION 3.1...3.18)

project(LibStftPitchShift)

include(GNUInstallDirs)

include("${CMAKE_CURRENT_LIST_DIR}/pocketfft/CMakeLists.txt")

if(SHARED)
  add_library(LibStftPitchShift SHARED)
else()
  add_library(LibStftPitchShift)
endif()

set_target_properties(LibStftPitchShift
  PROPERTIES SOVERSION 1
)

set_target_properties(LibStftPitchShift
  PROPERTIES OUTPUT_NAME "stftpitchshift"
)

set(HEADERS
  "${CMAKE_CURRENT_LIST_DIR}/Cepster.h"
  "${CMAKE_CURRENT_LIST_DIR}/Dump.h"
  "${CMAKE_CURRENT_LIST_DIR}/FFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/Pitcher.h"
  "${CMAKE_CURRENT_LIST_DIR}/Resampler.h"
  "${CMAKE_CURRENT_LIST_DIR}/RFFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/STFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.h"
  "${CMAKE_CURRENT_LIST_DIR}/Timer.h"
  "${CMAKE_CURRENT_LIST_DIR}/Version.h"
  "${CMAKE_CURRENT_LIST_DIR}/Vocoder.h"
)

set(SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/STFT.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.cpp"
)

target_sources(LibStftPitchShift
  PUBLIC ${HEADERS} ${SOURCES}
)

set_target_properties(LibStftPitchShift
  PROPERTIES PUBLIC_HEADER "${HEADERS}"
)

target_include_directories(LibStftPitchShift
  PUBLIC  "${CMAKE_CURRENT_LIST_DIR}/.."
  PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

target_link_libraries(LibStftPitchShift
  PUBLIC pocketfft
)

target_compile_features(LibStftPitchShift
  PRIVATE cxx_std_11
)

install(TARGETS LibStftPitchShift
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/StftPitchShift
)

# link against pthread on Unix required by pocketfft
if (UNIX)
  target_link_libraries(LibStftPitchShift
    PUBLIC pthread)
endif()
