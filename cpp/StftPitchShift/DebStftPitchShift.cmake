include("${CMAKE_CURRENT_LIST_DIR}/LibStftPitchShift.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/TheStftPitchShift.cmake")

set(CPACK_PACKAGE_NAME stftpitchshift)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "STFT based multi pitch shifting with optional formant preservation.")
set(CPACK_PACKAGE_VENDOR "Juergen Hock")
set(CPACK_PACKAGE_CONTACT "juergen.hock@jurihock.de")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Juergen Hock <juergen.hock@jurihock.de>")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/jurihock/stftPitchShift")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION)
string(STRIP "${VERSION}" VERSION)
set(CPACK_PACKAGE_VERSION "${VERSION}")

set(CPACK_GENERATOR DEB)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}")

set(CPACK_SOURCE_IGNORE_FILES
  "\.git/"
  "\.github/"
  "build.*/"
  "cmake-build.*/"
  "_CPack_.*/"
  ".*\.deb$"
  ".*\.tar\.gz$"
  "dist/"
  ".*\.egg-info/"
  "__pycache__/"
  ".*\.py[cod]$"
  "\.DS_Store"
  "Thumbs\.db"
  "voice\..*\.wav"
)

string(TIMESTAMP DATE "%a, %d %b %Y %H:%M:%S +0000" UTC)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/changelog.template"
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/changelog"
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/control.template"
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/control"
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/copyright"
  COPYONLY
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/rules.template"
  "${CMAKE_CURRENT_SOURCE_DIR}/debian/rules"
)

# FIX https://lintian.debian.org/tags/no-changelog
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/changelog.gz"
  COMMAND gzip -cn9 "${CMAKE_CURRENT_SOURCE_DIR}/debian/changelog" > "${CMAKE_CURRENT_BINARY_DIR}/changelog.gz"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/debian/changelog"
)
add_custom_target(changelog
  ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/changelog.gz"
)
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/changelog.gz"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/${CPACK_PACKAGE_NAME}"
)

# FIX https://lintian.debian.org/tags/no-copyright-file
install(
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
  DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/${CPACK_PACKAGE_NAME}"
  RENAME "copyright"
)

file(GLOB_RECURSE
  LICENSES "${CMAKE_CURRENT_LIST_DIR}/*/LICENSE"
)

foreach(LICENSE ${LICENSES})
  get_filename_component(DIRECTORY "${LICENSE}" DIRECTORY)
  get_filename_component(NAME "${DIRECTORY}" NAME)
  install(
    FILES "${CMAKE_CURRENT_LIST_DIR}/${NAME}/LICENSE"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/${CPACK_PACKAGE_NAME}"
    RENAME "copyright.${NAME}"
  )
endforeach()

# FIX https://lintian.debian.org/tags/no-shlibs
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ${BUILD_SHARED_LIBS})

include(CPack)
