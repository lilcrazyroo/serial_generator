cmake_minimum_required( VERSION 3.0 )

project( SERIAL_GENERATOR LANGUAGES CXX )
set( CMAKE_CXX_STANDARD 14 )

add_executable( serial_generator serial_generator.cpp )

target_compile_definitions( serial_generator PRIVATE 
    -DDISABLE_SERIALIZATION 
    -DDISABLE_SERIAL_TRACE 
    -DSERIAL_FORWARD_HEADER="${SERIAL_FORWARD_HEADER}" )

target_include_directories( serial_generator PRIVATE 
    ${SERIAL_SOURCE_DIR}/thirdparty/memserial/include
    ${SERIAL_SOURCE_DIR}/thirdparty )
        
add_custom_target( serial_generated ALL
    COMMAND serial_generator ${SERIAL_GENERATED_HEADER}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} )
    
add_dependencies( serial_generated serial_generator )
