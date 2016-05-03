// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/pair.hpp>
#include <ecst/mp/map/basic.hpp>
#include <ecst/mp/map/picker.hpp>

ECST_MP_MAP_NAMESPACE
{
    namespace impl
    {
        template <typename TMap, typename TPicker>
        constexpr auto list_builder(TMap m, TPicker p)
        {
            return list::transform(p, m);
        }
    }

    /// @brief Returns a list containing all the keys of `m`.
    template <typename TMap>
    constexpr auto key_list(TMap m)
    {
        return decltype(impl::list_builder(m, impl::picker_key())){};
    }

    /// @brief Returns a list containing all the values of `m`.
    template <typename TMap>
    constexpr auto value_list(TMap m)
    {
        return decltype(impl::list_builder(m, impl::picker_value())){};
    }

    /// @brief Returns the number of keys matching `p`.
    template <typename TMap, typename TPredicate>
    constexpr auto count_keys_matching(TMap m, TPredicate p)
    {
        return list::count_matching(key_list(m), p);
    }

    /// @brief Returns the number of values matching `p`.
    template <typename TMap, typename TPredicate>
    constexpr auto count_values_matching(TMap m, TPredicate p)
    {
        return list::count_matching(value_list(m), p);
    }

    /// @brief Returns true if any key matches `p`.
    template <typename TMap, typename TPredicate>
    constexpr auto any_key_matching(TMap m, TPredicate p)
    {
        return bool_v<(count_keys_matching(m, p) > 0)>;
    }

    /// @brief Returns true if any value matches `p`.
    template <typename TMap, typename TPredicate>
    constexpr auto any_value_matching(TMap m, TPredicate p)
    {
        return bool_v<(count_values_matching(m, p) > 0)>;
    }

    /// @brief Returns true if all keys match `p`.
    template <typename TMap, typename TPredicate>
    constexpr auto all_keys_matching(TMap m, TPredicate p)
    {
        return bool_v<(count_keys_matching(m, p) == size(m))>;
    }

    /// @brief Returns true if all values match `p`.
    template <typename TMap, typename TPredicate>
    constexpr auto all_values_matching(TMap, TPredicate p)
    {
        return bool_v<(count_values_matching(TMap{}, p) == size(TMap{}))>;
    }
}
ECST_MP_MAP_NAMESPACE_END
