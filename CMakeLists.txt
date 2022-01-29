cmake_minimum_required(VERSION 3.0)

project(HELLO)

include_directories(./depends/include)
include_directories(./src/common)
link_directories(./depends/libs)

add_subdirectory(./src/common)
add_subdirectory(./src/mergeblocks)
add_subdirectory(./src/kmeans)
add_subdirectory(./src/kmeansxor)


set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(${CMAKE_SOURCE_DIR}/src/common SRC)

add_executable(hello hello.cpp ${SRC})