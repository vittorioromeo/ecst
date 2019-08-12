# Copyright (c) 2015-2016 Vittorio Romeo
# License: Academic Free License ("AFL") v. 3.0
# AFL License page: http://opensource.org/licenses/AFL-3.0
# http://vittorioromeo.info | vittorio.romeo@outlook.com

cmake_minimum_required(VERSION 3.0)

# Include `vrm_cmake`.
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/../vrm_cmake/cmake/"
    "${CMAKE_SOURCE_DIR}/extlibs/vrm_cmake/cmake/")

include(vrm_cmake)

# Basic project setup.
vrm_cmake_init_project(ecst)
vrm_cmake_set_cxxstd(17)
vrm_cmake_find_extlib(vrm_pp)
vrm_cmake_find_extlib(vrm_core)

# TODO: improve/remove MinGW devel environment setup
# set(WIN_MINGW_DEVEL true)
if(DEFINED WIN_MINGW_DEVEL)
#{
    add_definitions("-DECST_USE_MEGANZ_MINGW_THREAD")

    find_package(SFML)
    find_package(Boost 1.60.0 COMPONENTS thread)

    link_directories("${CMAKE_SOURCE_DIR}/../SFML/lib/")

    include_directories("${CMAKE_SOURCE_DIR}/../SFML/include/")
    include_directories("${CMAKE_SOURCE_DIR}/../hana/include/")
    include_directories(${Boost_INCLUDE_DIRS})
#}
endif()

# Alias the include directory.
set(ECST_INC_DIR "${ECST_SOURCE_DIR}/include/")
include_directories("${ECST_INC_DIR}")

# TODO: comment
include_directories("/home/vittorioromeo/OHWorkspace/ecst/include")
include_directories("/home/vittorioromeo/OHWorkspace/ecst/include/")
include_directories("/home/vittorioromeo/OHWorkspace/ecst")

# Other compiler flags.
vrm_cmake_add_common_compiler_flags()

# The `check` target runs all tests and examples.
vrm_check_target()

# Setup subdirectories.
add_subdirectory(test)
add_subdirectory(example)

# Create header-only install target (automatically glob)
vrm_cmake_header_only_install_glob("${ECST_INC_DIR}" "include")

# vrm_cmake_add_common_compiler_flags_suggest_attribute()

add_executable(particles "${ECST_SOURCE_DIR}/example/particles.cpp")
add_executable(pres_code "${ECST_SOURCE_DIR}/example/pres_code.cpp")

if(DEFINED WIN_MINGW_DEVEL)
#{
    target_link_libraries(particles boost_system boost_thread)
    target_link_libraries(pres_code boost_system boost_thread sfml-system sfml-graphics sfml-window)
#}
else()
#{
    target_link_libraries(particles pthread)
    target_link_libraries(pres_code pthread sfml-system sfml-graphics sfml-window)
#}
endif()
