include("${CMAKE_CURRENT_LIST_DIR}/LibStftPitchShift.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/TheStftPitchShift.cmake")

set(CPACK_PACKAGE_NAME stftpitchshift)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "STFT based multi pitch shifting with optional formant preservation.")
set(CPACK_PACKAGE_VENDOR "Juergen Hock")
set(CPACK_PACKAGE_CONTACT "juergen.hock@jurihock.de")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Juergen Hock <juergen.hock@jurihock.de>")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VERSION)
string(STRIP "${VERSION}" VERSION)
set(CPACK_PACKAGE_VERSION "${VERSION}")

set(CPACK_GENERATOR DEB)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

include(CPack)
