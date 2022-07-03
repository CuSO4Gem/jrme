SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BUILD_RPATH}/bin) 

aux_source_directory(. SOURCE_CODES)
add_executable(jrme ${SOURCE_CODES})
target_link_libraries(jrme pthread dl)