vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO jurihock/stftPitchShift
  HEAD_REF main
  # REF 0
  # SHA512 0
)

set(OPTIONS -DVCPKG=ON)

if(VCPKG_LIBRARY_LINKAGE STREQUAL "static")
  list(APPEND OPTIONS -DSHARED=OFF)
endif()

if(VCPKG_LIBRARY_LINKAGE STREQUAL "dynamic")
  list(APPEND OPTIONS -DSHARED=ON)
endif()

vcpkg_configure_cmake(
  SOURCE_PATH ${SOURCE_PATH}
  PREFER_NINJA
  OPTIONS ${OPTIONS}
)

vcpkg_install_cmake()

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/stftpitchshift RENAME copyright)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
