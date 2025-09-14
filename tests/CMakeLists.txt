add_executable(
    test_pdf_page
    test_pdf_page.cpp   
)

target_link_libraries(
    test_pdf_page
    PRIVATE client 
)