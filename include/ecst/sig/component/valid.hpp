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
        struct valid_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts...) const noexcept
            {
                // TODO: why doesn't it work?
                // return std::conjunction_v<std::is_base_of<impl::base_data, Ts>...>;
                // return (std::is_base_of_v<impl::base_data, Ts> && ...);
                return mp::list::all_variadic(std::is_base_of<impl::base_data, Ts>{}...);
            }
        };
    } // namespace impl

    /// @brief Evaluates to true if all `Ts...` are component signatures.
    constexpr impl::valid_t valid{};
} // namespace ecst::sig::component
