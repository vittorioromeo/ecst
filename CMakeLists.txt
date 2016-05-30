# Copyright (c) 2015-2016 Vittorio Romeo
# License: Academic Free License ("AFL") v. 3.0
# AFL License page: http://opensource.org/licenses/AFL-3.0
# http://vittorioromeo.info | vittorio.romeo@outlook.com

cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Include SSVCmake.
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/../SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/")

include(SSVCMake)

# Basic project setup.
vrm_cmake_init_project(ecst)
vrm_cmake_find_extlib(vrm_pp)
vrm_cmake_find_extlib(vrm_core)

# TODO: remove
include_directories("/home/vittorioromeo/OHWorkspace/vrm_core/include")
# include_directories("D:/OHWorkspace/SFML/include")
# include_directories("D:/OHWorkspace/mingw-std-threads")
# link_directories("D:/OHWorkspace/SFML/lib")
# find_package(boost 1.36.0 COMPONENTS system thread)
# include_directories("${Boost_INCLUDE_DIRS}")

# Alias the include directory.
set(ECST_INC_DIR "${ECST_SOURCE_DIR}/include/")
include_directories("${ECST_INC_DIR}")

# Enable pthread support.
# vrm_cmake_add_compiler_flag_pthread()

# Other compiler flags.
vrm_cmake_add_common_compiler_flags()

# Setup user options.
vrm_cmake_add_option_memcheck()
vrm_cmake_add_option_no_exceptions()
vrm_cmake_add_option_werror()

# The `check` target runs all tests and examples.
vrm_check_target()

# Setup subdirectories.
add_subdirectory(test)
add_subdirectory(example)

# Create header-only install target (automatically glob)
vrm_cmake_header_only_install_glob("${ECST_INC_DIR}" "include")

# TODO: remove
# vrm_cmake_add_common_compiler_flags_suggest_attribute()

# TODO: remove
add_executable(particles "${ECST_SOURCE_DIR}/example/particles.cpp")
target_link_libraries(particles pthread)

add_executable(pres_code "${ECST_SOURCE_DIR}/example/pres_code.cpp")
# target_link_libraries(pres_code boost_system boost_thread sfml-system sfml-graphics sfml-window)
target_link_libraries(pres_code pthread sfml-system sfml-graphics sfml-window)
