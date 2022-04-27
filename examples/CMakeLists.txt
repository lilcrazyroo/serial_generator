add_executable( example1 example1.cpp )
target_include_directories( example1 PRIVATE 
    ${CMAKE_SOURCE_DIR}/thirdparty/memserial/include
    ${CMAKE_SOURCE_DIR}/thirdparty )

add_executable( example2 example2.cpp )
target_include_directories( example2 PRIVATE 
    ${CMAKE_SOURCE_DIR}/thirdparty/memserial/include
    ${CMAKE_SOURCE_DIR}/thirdparty )

add_executable( example3 example3.cpp )
target_link_libraries( example3 PRIVATE memserial )
