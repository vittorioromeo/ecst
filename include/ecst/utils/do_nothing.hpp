// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

namespace ecst
{
    namespace impl
    {
        /// @brief Empty struct with a `constexpr` variadic `operator()` that
        /// does nothing.
        struct do_nothing_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts&&...) const noexcept
            {
            }
        };

        /// @brief Instance of `do_nothing_t`.
        constexpr impl::do_nothing_t do_nothing{};
    } // namespace impl
} // namespace ecst
