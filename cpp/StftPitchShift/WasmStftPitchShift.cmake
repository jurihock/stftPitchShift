cmake_minimum_required(VERSION 3.1...3.18)

project(wasm)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/stftpitchshift.js"
  COMMAND emcc
          "${CMAKE_CURRENT_LIST_DIR}/wasm.cpp"
          "${CMAKE_CURRENT_LIST_DIR}/StftPitchShift.cpp"
          -I "${CMAKE_CURRENT_LIST_DIR}"
          -I "${CMAKE_CURRENT_LIST_DIR}/.."
          -o "${CMAKE_CURRENT_BINARY_DIR}/stftpitchshift.js"
          -s WASM=1
          -s EXPORTED_RUNTIME_METHODS=allocateUTF8,UTF8ToString
          -s EXPORTED_FUNCTIONS=_malloc,_free
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  DEPENDS "${CMAKE_CURRENT_LIST_DIR}/wasm.cpp" VERBATIM)

add_custom_target(${PROJECT_NAME}
  ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/stftpitchshift.js")
