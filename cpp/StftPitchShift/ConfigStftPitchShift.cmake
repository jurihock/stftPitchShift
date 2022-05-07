set(StftPitchShift stftpitchshift)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" StftPitchShiftVersion)
string(STRIP "${StftPitchShiftVersion}" StftPitchShiftVersion)

include(CMakePackageConfigHelpers)

install(
  TARGETS  ${StftPitchShift}
  EXPORT   ${StftPitchShift}Targets
  LIBRARY  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  RUNTIME  DESTINATION "${CMAKE_INSTALL_BINDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/StftPitchShift"
)

install(
  EXPORT      ${StftPitchShift}Targets
  FILE        ${StftPitchShift}Targets.cmake
  NAMESPACE   ${StftPitchShift}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${StftPitchShift}"
)

configure_package_config_file(
  "${CMAKE_CURRENT_LIST_DIR}/ConfigStftPitchShift.cmake.template"
  "${CMAKE_CURRENT_BINARY_DIR}/${StftPitchShift}Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${StftPitchShift}"
)

write_basic_package_version_file(
  "${StftPitchShift}ConfigVersion.cmake"
  VERSION "${StftPitchShiftVersion}"
  COMPATIBILITY AnyNewerVersion
)

install(
  FILES       "${CMAKE_CURRENT_BINARY_DIR}/${StftPitchShift}Config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${StftPitchShift}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${StftPitchShift}"
)
