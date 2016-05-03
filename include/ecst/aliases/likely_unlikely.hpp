// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/core/utility_macros/likely_unlikely.hpp>

/// @brief Micro-optimization: condition likely to be true.
#define ECST_LIKELY(...) VRM_CORE_LIKELY(__VA_ARGS__)

/// @brief Micro-optimization: condition unlikely to be true.
#define ECST_UNLIKELY(...) VRM_CORE_UNLIKELY(__VA_ARGS__)
