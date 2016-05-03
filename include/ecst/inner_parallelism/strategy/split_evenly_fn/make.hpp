// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/hardware.hpp>
#include "./parameters.hpp"

ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE
{
    namespace split_evenly_fn
    {
        template <                       // .
            typename TSubtaskCountGetter // .
            >
        constexpr auto v(TSubtaskCountGetter)
        {
            return impl::parameters< // .
                TSubtaskCountGetter  // .
                >{};
        }

        struct v_cores_getter
        {
            auto operator()() const noexcept
            {
                ECST_ASSERT(ecst::hardware::status::core_count_known());
                return ecst::hardware::status::core_count();
            }
        };

        constexpr auto v_cores()
        {
            return v(v_cores_getter{});
        }
    }
}
ECST_INNER_PARALLELISM_STRATEGY_NAMESPACE_END
