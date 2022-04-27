cmake_minimum_required( VERSION 3.0 )

project( SERIAL_GENERATOR LANGUAGES CXX )
set( CMAKE_CXX_STANDARD 14 )

add_executable( serial_generator serial_generator.cpp )

target_compile_definitions( serial_generator PRIVATE 
    -DDISABLE_SERIALIZATION 
    -DDISABLE_SERIAL_TRACE 
    -DSERIAL_FORWARD_HEADER="serial_precompile.cpp" )

target_include_directories( serial_generator PRIVATE 
    ${CMAKE_ROOT_DIR}/thirdparty/memserial/include
    ${CMAKE_ROOT_DIR}/thirdparty )
        
add_custom_target( serial_generated ALL
    COMMAND serial_generator serial_generated.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
    
add_dependencies( serial_generated serial_generator )
