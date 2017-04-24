# Copyright (c) 2015-2016 Vittorio Romeo
# License: Academic Free License ("AFL") v. 3.0
# AFL License page: http://opensource.org/licenses/AFL-3.0
# http://vittorioromeo.info | vittorio.romeo@outlook.com

# Adapted from Louise Dionne's FindHana.cmake file

# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

find_path(
    ECST_INCLUDE_DIR

    NAMES ecst/include/ecst.hpp
    DOC "Include directory for the ecst library"

    PATH_SUFFIXES include/

    PATHS
        "${PROJECT_SOURCE_DIR}/../ecst/"
        ${ECST_ROOT}
        $ENV{ECST_ROOT}
        /usr/local/
        /usr/
        /sw/
        /opt/local/
        /opt/csw/
        /opt/
        "${PROJECT_SOURCE_DIR}/extlibs/ecst/"
        "${CMAKE_CURRENT_LIST_DIR}/../../"

        NO_DEFAULT_PATH
)

if (NOT EXISTS "${ECST_INCLUDE_DIR}" AND DEFINED ECST_CLONE_DIR)
    set(_build_dir "${CMAKE_CURRENT_BINARY_DIR}/ecst")

    if (DEFINED ECST_ENABLE_TESTS)
        set(_test_cmd ${CMAKE_COMMAND} --build ${_build_dir} --target check)
    else()
        set(_test_cmd "")
    endif()

    include(ExternalProject)
    ExternalProject_Add(ecst
        PREFIX ${_build_dir}
        STAMP_DIR ${_build_dir}/_stamps
        TMP_DIR ${_build_dir}/_tmp

        # Since we don't have any files to download, we set the DOWNLOAD_DIR
        # to TMP_DIR to avoid creating a useless empty directory.
        DOWNLOAD_DIR ${_build_dir}/_tmp

        # Download step
        GIT_REPOSITORY https://github.com/SuperV1234/ecst
        GIT_TAG master
        TIMEOUT 20

        # Configure step
        SOURCE_DIR "${ECST_CLONE_DIR}"
        CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}

        BINARY_DIR "${_build_dir}"
        BUILD_COMMAND ""

        # Install step (nothing to be done)
        INSTALL_COMMAND ""

        # Test step
        TEST_COMMAND ${_test_cmd}
    )

    set(ECST_INCLUDE_DIR "${ECST_CLONE_DIR}/include")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ECST DEFAULT_MSG ECST_INCLUDE_DIR)