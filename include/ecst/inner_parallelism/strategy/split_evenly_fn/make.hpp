// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./parameters.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/hardware.hpp>

namespace ecst::inner_parallelism::strategy
{
    namespace split_evenly_fn
    {
        template <typename TSubtaskCountGetter>
        constexpr auto v(TSubtaskCountGetter)
        {
            return impl::parameters<TSubtaskCountGetter>{};
        }

        namespace impl
        {
            struct v_cores_getter
            {
                auto operator()() const noexcept
                {
                    ECST_ASSERT(ecst::hardware::status::core_count_known());
                    return ecst::hardware::status::core_count();
                }
            };
        } // namespace impl

        constexpr auto v_cores()
        {
            return v(impl::v_cores_getter{});
        }
    } // namespace split_evenly_fn
} // namespace ecst::inner_parallelism::strategy
