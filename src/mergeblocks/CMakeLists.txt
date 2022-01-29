

aux_source_directory(${CMAKE_SOURCE_DIR}/src/common SRC_COMMON)
aux_source_directory(. SRC)

set(CMAKE_BUILD_TYPE Debug)
# add_executable(run merge.cpp ${SRC})
add_executable(merge_run ${SRC} ${SRC_COMMON})