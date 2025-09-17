find_package(jsoncpp REQUIRED)
add_executable(
    test_pdf_page
    test_pdf_page.cpp   
)

target_link_libraries(
    test_pdf_page
    PRIVATE client 
)

add_executable(
    test_json
    test_json.cpp
)

target_link_libraries(
    test_json
    PRIVATE jsoncpp_static
)