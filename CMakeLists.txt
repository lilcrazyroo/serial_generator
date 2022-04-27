cmake_minimum_required( VERSION 3.0 )

project( MEMSERIAL LANGUAGES CXX )
set( CMAKE_CXX_STANDARD 14 )
 
add_library( serial_specs INTERFACE )
target_compile_definitions( serial_specs INTERFACE -DSERIAL_ENDIAN_DEFAULT=BigEndian )
target_include_directories( serial_specs INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/memserial/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty )
 
if( FALSE )
    find_package( Qt5 QUIET COMPONENTS Core )
    target_link_libraries( serial_specs INTERFACE Qt5::Core )
endif()

if( TRUE )
    target_compile_definitions( serial_specs INTERFACE -DSERIAL_REBIND_HEADER="serial_generated.h" )
        
    add_custom_command( OUTPUT ${CMAKE_CURRENT_LIST_DIR}/src/serial_generated.h 
        COMMAND ${CMAKE_COMMAND} -D CMAKE_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -S ${CMAKE_CURRENT_SOURCE_DIR}/src -B ${CMAKE_CURRENT_BINARY_DIR}/serial_generator 
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/serial_generator
        BYPRODUCTS ${CMAKE_CURRENT_LIST_DIR}/src/serial_generated.h
        IMPLICIT_DEPENDS CXX ${CMAKE_CURRENT_LIST_DIR}/src/serial_precompile.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src )
        
    add_custom_target( serial_rebind DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/serial_generated.h )
    add_dependencies( serial_specs serial_rebind )
endif()

if( TRUE )
	add_library( memserial SHARED src/serial_precompile.cpp )
    set_target_properties( memserial PROPERTIES LINKER_LANGUAGE CXX )
    target_link_libraries( memserial PUBLIC serial_specs )
    target_include_directories( memserial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src )
    target_compile_definitions( memserial PRIVATE -DENABLE_SERIAL_VERSION )
endif()

if( TRUE )
    add_subdirectory( examples )
endif()
