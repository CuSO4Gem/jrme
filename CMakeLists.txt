project(jrme)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDBG_PRINT_ENABLED=4")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDBG_PRINT_ENABLED=4")
    message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

include_directories(include source)

add_subdirectory(source)
add_subdirectory(plugin)
add_subdirectory(googletest)
add_subdirectory(test)