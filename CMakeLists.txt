cmake_minimum_required(VERSION 3.15)

project(
    tang
    LANGUAGES C CXX
    VERSION 1.0.0
)

# for qt!
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# disable the cxx extension
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# set utf8
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# find qt
set(QT_SDK_DIR "" CACHE PATH "Qt sdk dir" FORCE)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets )
find_package(
    Qt${QT_VERSION_MAJOR} REQUIRED 
    COMPONENTS 
        Widgets 
        PdfWidgets
        Pdf
        Charts
)

# find ela

find_package(ElaWidgetTools REQUIRED)


add_subdirectory("client")
# add_subdirectory("tests")
