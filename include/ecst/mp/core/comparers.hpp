// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core/aliases.hpp>
#include <type_traits>

ECST_MP_NAMESPACE
{
    namespace impl
    {
        // `constexpr` callable returning whether or not two objects have the
        // same type.
        struct same_type
        {
            template <typename T0, typename T1>
            constexpr auto operator()(T0&&, T1&&) const noexcept
            {
                return std::is_same<T0, T1>{};
            }
        };

        // `constexpr` callable returning whether or not two objects have the
        // same type, after applying `decay_t` to both of them.
        struct same_type_decay
        {
            template <typename T0, typename T1>
            constexpr auto operator()(T0&&, T1&&) const noexcept
            {
                return same_type{}(decay_t<T1>{}, decay_t<T0>{});
            }
        };
    }

    // Variable template aliases.
    constexpr impl::same_type same_type{};
    constexpr impl::same_type_decay same_type_decay{};
}
ECST_MP_NAMESPACE_END
