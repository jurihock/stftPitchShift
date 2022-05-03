vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO jurihock/stftPitchShift
  REF f40e3b63540aacc1526155c7962342a5fe3245c5
  SHA512 39dfb637408d78f9163232080c9a55ee9b8a3b1b887677a2c8f4085eb5cbdf40b239d947e5eb415521f50b81f1342d8c242f6473c8673b0b02ddc2ac54461c62
  HEAD_REF master
)

if(VCPKG_LIBRARY_LINKAGE STREQUAL "static")

  vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
      -DVCPKG=ON
      -DSHARED=OFF
  )

endif()

if(VCPKG_LIBRARY_LINKAGE STREQUAL "dynamic")

  vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
      -DVCPKG=ON
      -DSHARED=ON
  )

endif()

vcpkg_install_cmake()

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/stftpitchshift RENAME copyright)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
