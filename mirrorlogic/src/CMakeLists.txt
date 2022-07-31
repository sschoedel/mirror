include_directories (${HEXMIRROR_SOURCE_DIR}/mirrorlogic/include)
include_directories (${HEXMIRROR_SOURCE_DIR}/libs/vis/include)
link_directories (${HEXMIRROR_SOURCE_DIR}/libs/vis/src)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable (hexmirror ../main.cpp mirrorgrid.cpp tile.cpp)
target_link_libraries (hexmirror LibMirrorGridVis)