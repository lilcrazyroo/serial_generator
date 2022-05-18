cmake_minimum_required( VERSION 3.2 )

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
    string( TOLOWER "serial_generated.${CMAKE_CXX_COMPILER_ID}.${CMAKE_SYSTEM_PROCESSOR}.h" GENERATED_HEADER )
    target_compile_definitions( serial_specs INTERFACE -DSERIAL_REBIND_HEADER="${GENERATED_HEADER}" )
        
    add_custom_command( OUTPUT ${CMAKE_CURRENT_LIST_DIR}/src/${GENERATED_HEADER} 
        COMMAND ${CMAKE_COMMAND} -B${CMAKE_CURRENT_BINARY_DIR}/serial_generator ${CMAKE_CURRENT_SOURCE_DIR}/src
            -DSERIAL_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
            -DSERIAL_FORWARD_HEADER="serial_precompile.cpp"
            -DSERIAL_GENERATED_HEADER=${GENERATED_HEADER}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/serial_generator
        BYPRODUCTS ${CMAKE_CURRENT_LIST_DIR}/src/${GENERATED_HEADER}
        IMPLICIT_DEPENDS CXX ${CMAKE_CURRENT_LIST_DIR}/src/serial_precompile.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src )
        
    add_custom_target( serial_generated DEPENDS ${CMAKE_CURRENT_LIST_DIR}/src/${GENERATED_HEADER} )
    add_dependencies( serial_specs serial_generated )
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
