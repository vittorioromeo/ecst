// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>

namespace ecst::sig::component
{
    namespace impl
    {
        template <typename T>
        using valid_impl = std::is_base_of<impl::base_data, T>;

        struct valid_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts...) const noexcept
            {
                return mp::list::all_variadic(impl::valid_impl<Ts>{}...);
            }
        };
    } // namespace impl

    /// @brief Evaluates to true if all `Ts...` are component signatures.
    constexpr impl::valid_t valid{};
} // namespace ecst::sig::component
