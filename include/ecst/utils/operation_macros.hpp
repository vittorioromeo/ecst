// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

/// @brief Defines deleted copy operations and default move operations for the
/// class `name`.
#define ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(name) \
    name(const name&) = delete;                        \
    name& operator=(const name&) = delete;             \
                                                       \
    name(name&&) = default;                            \
    name& operator=(name&&) = default
