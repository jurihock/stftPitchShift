vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO jurihock/stftPitchShift
  HEAD_REF main
  # REF 0
  # SHA512 0
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DVCPKG=ON
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
  PACKAGE_NAME "stftpitchshift"
  CONFIG_PATH "lib/cmake/stftpitchshift"
)

file(
  INSTALL "${SOURCE_PATH}/LICENSE"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/stftpitchshift"
  RENAME copyright
)

file(
  REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include"
)
