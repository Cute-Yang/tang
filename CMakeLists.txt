cmake_minimum_required(VERSION 3.15)

project(
    tang
    LANGUAGES C CXX
    VERSION 1.0.0
)


# disable the cxx extension
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# set utf8
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory("server")
add_subdirectory("client")
add_subdirectory("tests")
