# Include directories.
include_directories(${ECST_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE INSTALL_FILES_LIST "${CMAKE_CURRENT_LIST_DIR}/*")
set_source_files_properties(${INSTALL_FILES_LIST} PROPERTIES HEADER_FILE_ONLY 1)
add_library(HEADER_ONLY_TARGET2 STATIC ${INSTALL_FILES_LIST})
set_target_properties(HEADER_ONLY_TARGET2 PROPERTIES LINKER_LANGUAGE CXX)
install(DIRECTORY ${src_dir} DESTINATION ${dest_dir})
