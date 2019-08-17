// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/sig/system/data.hpp>

namespace ecst::sig::system
{
    namespace impl
    {
        struct valid_t
        {
            template <typename... Ts>
            constexpr auto operator()(Ts...) const noexcept
            {
                return mp::list::all_variadic(
                    is_specialization_of_t<Ts, impl::data>{}...);
            }
        };
    } // namespace impl

    /// @brief Evaluates to true if all `Ts...` are system signatures.
    constexpr impl::valid_t valid{};
} // namespace ecst::sig::system
