set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugin)

#  插件不需要以lib为前缀
#  Plugins not need to prefix with "lib"
set(CMAKE_SHARED_LIBRARY_PREFIX "")

add_subdirectory(save_date)
add_subdirectory(md_journal)