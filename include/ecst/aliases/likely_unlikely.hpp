// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/core/utility_macros/likely_unlikely.hpp>
#include <ecst/config/names.hpp>
#include <ecst/aliases/attributes.hpp>

// TODO: implement as functions in vrm_core as well
ECST_NAMESPACE
{
    /// @brief Micro-optimization: condition likely to be true.
    ECST_ALWAYS_INLINE auto likely(bool x) noexcept
    {
        return VRM_CORE_LIKELY(x);
    }

    /// @brief Micro-optimization: condition unlikely to be true.
    ECST_ALWAYS_INLINE auto unlikely(bool x) noexcept
    {
        return VRM_CORE_UNLIKELY(x);
    }
}
ECST_NAMESPACE_END
