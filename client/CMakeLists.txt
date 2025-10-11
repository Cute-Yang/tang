cmake_minimum_required(VERSION 3.15)
set(client_lib_name "tang_client")

# for qt!
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#*************************************************************
option(JUST_FOR_TEST "jsut for test..." OFF)
if(JUST_FOR_TEST)
    set(CMAKE_PREFIX_PATH "D:/dev_libs/ela_widget/debug_test/ElaWidgetTools/lib")
    include_directories("D:/work/tang")
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
endif(JUST_FOR_TEST)
#***************************************************************

find_package(Qt6 REQUIRED COMPONENTS
    Widgets
    PdfWidgets
    Pdf
    Charts
    Network
    WebSockets
)

find_package(ElaWidgetTools REQUIRED)

# find all the files...
file(GLOB_RECURSE RES_PATHS *.png *.jpg *.svg *.ico *.ttf *.webp *.js)

# foreach(filepath ${RES_PATHS})
#     string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
#     list(APPEND resource_files ${filename})
#     message(STATUS ${filename})
# endforeach(filepath)

FILE(GLOB sources CONFIGURE_DEPENDS "*.h" "*.cpp")
add_library(
    ${client_lib_name} STATIC
    ${sources}
)
FILE(GLOB resources CONFIGURE_DEPENDS "*.qrc")
set(exe_icon_file "cat_icon.rc")

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
    PUBLIC Qt6::Widgets
    PUBLIC Qt6::PdfWidgets
    PUBLIC Qt6::Charts
    PUBLIC Qt6::Network
    PUBLIC Qt6::WebSockets
)



function(add_client_test source_file add_icon)
    get_filename_component(target_name ${source_file} NAME_WE)
    message(STATUS "add target ${target_name} from source file ${source_file}")
    if(add_icon AND MSVC)
        add_executable(${target_name} ${source_file} ${resources} ${exe_icon_file})
    else()
        add_executable(${target_name} ${source_file} ${resources})
    endif()
    target_link_libraries(${target_name} ${client_lib_name})
endfunction(add_client_test)

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
    "water/test_ws.cpp"
    "water/test_chat_ui.cpp"
    "water/test_chat_ui_v2.cpp"
    "water/test_chat.cpp"
    "water/test.cpp"
)

foreach(source ${client_test_sources})
    add_client_test(${source} OFF)
endforeach()

add_client_test("water/vote_client.cpp" ON)
set_target_properties(vote_client PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
