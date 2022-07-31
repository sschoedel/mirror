cmake_minimum_required (VERSION 3.0)
project (HEXMIRROR)


add_subdirectory (libs/vis)
add_subdirectory (mirrorlogic)

# set (LIBS
#     ${LIBNAME1}
#     ${LIBNAME2}
# )

# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# add_executable (hexmirror main.cpp)
# target_include_directories (hexmirror PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/logic/include)
# target_include_directories (hexmirror PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libs/vis/include)
# target_link_libraries (hexmirror LINK_PUBLIC ${LIBS})