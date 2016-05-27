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

/// @brief Defines an unused struct with an unique name, that requires an
/// user-provided semicolon to compile.
#define ECST_IMPL_SEMICOLON_SUPPRESSOR()                                     \
    struct VRM_CORE_UNUSED_FN VRM_PP_CAT(ss, __LINE__, semicolon_suppressor) \
    {                                                                        \
    }

/// @brief Defines an unused function with an unique name. The body must be
/// provided by the user.
#define ECST_IMPL_UNUSED_FN(...)                      \
    auto VRM_CORE_UNUSED_FN ECST_CONST_FN VRM_PP_CAT( \
        ws, __LINE__, warning_suppressor, __VA_ARGS__)()
