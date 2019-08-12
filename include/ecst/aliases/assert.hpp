// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/core/assert.hpp>

/// @macro Runtime assertion. (no message)
#define ECST_ASSERT_NS(...) assert(__VA_ARGS__)

/// @macro Runtime assertion.
#define ECST_ASSERT(...) VRM_CORE_ASSERT(__VA_ARGS__)

/// @macro Runtime assertion of a binary operation.
#define ECST_ASSERT_OP(...) VRM_CORE_ASSERT_OP(__VA_ARGS__)

/// @macro Statically asserts the passed variadic arguments, after wrapping them
/// in `decltype(...){}`.
#define ECST_S_ASSERT_DT(...) static_assert(decltype(__VA_ARGS__){})
