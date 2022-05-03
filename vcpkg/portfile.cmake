vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO jurihock/stftPitchShift
  REF 41bf6ee65f85c0e496c2c9487e3f468a71d5c714
  SHA512 0071fa74edee788ba45e68f4d0d0c44715cb85f8ec9684a3092006ed9f2de2ebb61f3a081fb6e2c5c697eb2f125911184e5b868a25816a3cca7044fe8e7e3e1e
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
