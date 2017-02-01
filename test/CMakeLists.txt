# Add a custom target for the unit tests.
add_custom_target(tests COMMENT "Build all the unit tests.")

# `check` depends on `tests`.
add_dependencies(check tests)

# Include directories.
include_directories(${ECST_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE INSTALL_FILES_LIST "${CMAKE_CURRENT_LIST_DIR}/ecst/testbed/*.hpp")
file(GLOB_RECURSE INSTALL_FILES_LIST "${CMAKE_CURRENT_LIST_DIR}/ecst/*.hpp")
add_library(HEADER_ONLY_TARGET3 STATIC ${INSTALL_FILES_LIST})
set_source_files_properties(${INSTALL_FILES_LIST} PROPERTIES HEADER_FILE_ONLY 1)
set_target_properties(HEADER_ONLY_TARGET3 PROPERTIES LINKER_LANGUAGE CXX)

# Generate all the header unit tests.
# vrm_cmake_generate_public_header_tests_glob("*.hpp" "${ECST_SOURCE_DIR}/include")

# # Link `pthread` to every generated header test.
# foreach(_x IN LISTS vrm_cmake_out)
# #{
#     target_link_libraries(${_x} pthread)
# #}
# endforeach()

# Generate all the unit tests.
vrm_cmake_generate_unit_tests_glob("${ECST_SOURCE_DIR}/test/*.cpp")

macro(pthread_pls)
target_link_libraries(${vrm_cmake_out} pthread)
endmacro()

# Link `pthread` to every generated test.
foreach(_x IN LISTS vrm_cmake_out)
#{
    target_compile_options(${_x} PUBLIC "-fdiagnostics-color=always")

    if(${AUGMENT_TESTS})
    #{
        vrm_cmake_augment_test_asan(${_x})
        pthread_pls()

        vrm_cmake_augment_test_ofast(${vrm_cmake_out})
        pthread_pls()

        vrm_cmake_augment_test_ubsan(${_x})
        pthread_pls()

        vrm_cmake_augment_test_ofast(${vrm_cmake_out})
        pthread_pls()

        vrm_cmake_augment_test_tsan(${_x})
        pthread_pls()

        vrm_cmake_augment_test_ofast(${vrm_cmake_out})
        pthread_pls()
    #}
    endif()

    if(DEFINED WIN_MINGW_DEVEL)
    #{
        target_link_libraries(${_x} boost_system boost_thread)
    #}
    else()
    #{
        target_link_libraries(${_x} pthread)
    #}
    endif()
#}
endforeach()
