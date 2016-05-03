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
            constexpr auto operator()(T0, T1) const noexcept
            {
                return std::is_same<T0, T1>{};
            }
        };

        // `constexpr` callable returning whether or not two objects have the
        // same type, after applying `std::decay_t` to both of them.
        struct same_type_decay
        {
            template <typename T0, typename T1>
            constexpr auto operator()(T0, T1) const noexcept
            {
                return std::is_same<std::decay_t<T1>, std::decay_t<T0>>{};
            }
        };

        // `constexpr` callable that checks if an object has type equal to
        // `TItem`.
        template <typename TItem>
        struct equal_to
        {
            template <typename T>
            constexpr auto operator()(T x) const noexcept
            {
                return std::is_same<decltype(x), TItem>{};
            }
        };

        // Dummy type representing search/lookup failure.
        struct null
        {
            constexpr bool operator==(const null&) const noexcept
            {
                return true;
            }

            constexpr bool operator!=(const null&) const noexcept
            {
                return false;
            }
        };
    }

    // Variable template aliases.
    constexpr impl::same_type same_type{};
    constexpr impl::same_type_decay same_type_decay{};
    constexpr impl::null null_v{};

    template <typename TItem>
    constexpr impl::equal_to<TItem> equal_to{};

    template <typename T>
    constexpr auto is_null(T) noexcept
    {
        return bool_v<same_type_decay(T{}, null_v)>;
    }
}
ECST_MP_NAMESPACE_END
