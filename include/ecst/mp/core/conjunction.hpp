// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/mp/core/aliases.hpp>

ECST_MP_NAMESPACE
{
    namespace impl
    {
        template <typename...>
        struct conjunction : std::true_type
        {
        };

        template <typename T>
        struct conjunction<T> : T
        {
        };

        template <typename T, typename... Ts>
        struct conjunction<T, Ts...>
            : std::conditional_t<T::value != false, conjunction<Ts...>, T>
        {
        };

        template <typename... Ts>
        constexpr bool conjunction_v = conjunction<Ts...>::value;
    }

    template <typename... Ts>
    constexpr auto conjugate(Ts...) noexcept
    {
        return bool_v<impl::conjunction_v<Ts...>>;
    }
}
ECST_MP_NAMESPACE_END
