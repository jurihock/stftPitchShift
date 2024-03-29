cmake_minimum_required(VERSION 3.1...3.18)

project(stftpitchshift)

include(GNUInstallDirs)

add_library(${PROJECT_NAME})

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION)
string(STRIP "${VERSION}" VERSION)

set_target_properties(${PROJECT_NAME}
  PROPERTIES VERSION "${VERSION}"
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES SOVERSION 2
)

set_property(
  TARGET ${PROJECT_NAME}
  PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION 2)

set_property(
  TARGET ${PROJECT_NAME} APPEND
  PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)

set_target_properties(${PROJECT_NAME}
  PROPERTIES OUTPUT_NAME "stftpitchshift"
)

set(HEADERS
  "${CMAKE_CURRENT_LIST_DIR}/Arctangent.h"
  "${CMAKE_CURRENT_LIST_DIR}/Cepster.h"
  "${CMAKE_CURRENT_LIST_DIR}/Dump.h"
  "${CMAKE_CURRENT_LIST_DIR}/FFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/Normalizer.h"
  "${CMAKE_CURRENT_LIST_DIR}/Pitcher.h"
  "${CMAKE_CURRENT_LIST_DIR}/Resampler.h"
  "${CMAKE_CURRENT_LIST_DIR}/RFFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/STFT.h"
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.h"
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShiftCore.h"
  "${CMAKE_CURRENT_LIST_DIR}/Timer.h"
  "${CMAKE_CURRENT_LIST_DIR}/Version.h"
  "${CMAKE_CURRENT_LIST_DIR}/Vocoder.h"
)

set(SOURCES
  "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.cpp"
)

target_sources(${PROJECT_NAME}
  PRIVATE ${HEADERS} ${SOURCES}
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES PUBLIC_HEADER "${HEADERS}"
)

target_include_directories(${PROJECT_NAME}
  PUBLIC    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>"
  INTERFACE "$<INSTALL_INTERFACE:include/StftPitchShift>"
)

target_compile_features(${PROJECT_NAME}
  PRIVATE cxx_std_20
)

if(FASTMATH)

  message(STATUS "Enabling fast math")

  if(MSVC)
    target_compile_options(${PROJECT_NAME}
      PRIVATE /fp:fast)
  else()
    target_compile_options(${PROJECT_NAME}
      PRIVATE -ffast-math)
  endif()

endif()

if(FASTATAN)

  message(STATUS "Defining ENABLE_ARCTANGENT_APPROXIMATION")

  target_compile_definitions(${PROJECT_NAME}
    PRIVATE -DENABLE_ARCTANGENT_APPROXIMATION)

endif()

install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/StftPitchShift"
)
