// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <vrm/core/config.hpp>

ECST_HARDWARE_NAMESPACE
{
    namespace status
    {
        /// @brief Returns the CPU's core count.
        /// @details Returns `0` if the count is unknown.
        ECST_ALWAYS_INLINE auto core_count() noexcept
        {
            return ecst::thread::hardware_concurrency();
        }

        /// @brief Returns true if the CPU's core count is known.
        ECST_ALWAYS_INLINE auto core_count_known() noexcept
        {
            return core_count() > 0;
        }
    } // namespace status
}
ECST_HARDWARE_NAMESPACE_END
