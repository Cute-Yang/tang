set(client_lib_name "client")
# for qt!
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(QT_SDK_DIR "" CACHE PATH "Qt sdk dir" FORCE)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(
    Qt${QT_VERSION_MAJOR} REQUIRED 
    COMPONENTS 
        Widgets 
        PdfWidgets
        Pdf
        Charts
        Network
)
find_package(ElaWidgetTools REQUIRED)
# find all the files...
file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)
foreach (filepath ${RES_PATHS})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND resource_files ${filename})
    message(STATUS ${filename})
endforeach (filepath)


FILE(GLOB sources CONFIGURE_DEPENDS  "*.h" "*.cpp" "*.qrc")
# set(sources test_background.cpp)
message(STATUS ${sources})

add_library(
    ${client_lib_name} STATIC 
    ${sources}   
)

qt_add_resources(${client_lib_name} "ElaWidgetToolsExample"
    RESOURCES 
    PREFIX 
        "/icons"
    FILES
        ${resource_files}
)
target_include_directories(
    ${client_lib_name}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${ElaWidgetTools_INCLUDE_DIRS}
)

target_link_directories(
    ${client_lib_name}
    PUBLIC ${ElaWidgetTools_LIBRARY_DIRS}   
)

target_link_libraries(
    ${client_lib_name}
    PUBLIC ${ElaWidgetTools_LIBRARIES}
    PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
    PUBLIC Qt${QT_VERSION_MAJOR}::PdfWidgets
    PUBLIC Qt${QT_VERSION_MAJOR}::Charts
    PUBLIC Qt${QT_VERSION_MAJOR}::Network
)

function(add_client_test file_path)
    get_filename_component(file_name ${file_path} NAME_WE)
    message(STATUS "add target ${file_name} from source file ${file_path}")
    add_executable(
        ${file_name}
        ${file_path}   
    )
    target_link_libraries(
        ${file_name}
        PRIVATE
            ${client_lib_name}   
    )
endfunction( )

# add you test file here!
set(
    client_test_sources
    "water/test_pdf.cpp"
    "water/test_signin.cpp"
    "water/test_signup.cpp"
    "water/test_login.cpp"
    "water/test_login_setting.cpp"
    "water/test_main_window.cpp"
    "water/test_client.cpp"
    "water/test_pie_plot.cpp"
)

foreach(source ${client_test_sources})
    add_client_test(${source})
endforeach()





