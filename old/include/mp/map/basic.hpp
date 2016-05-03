// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/pair.hpp>
#include <ecst/mp/map/types.hpp>
#include <ecst/mp/map/is_map.hpp>

ECST_MP_MAP_NAMESPACE
{
    /// @brief Returns an empty map.
    constexpr auto make()
    {
        return list::make();
    }

    namespace impl
    {
        template <typename TMap>
        constexpr auto valid_impl(TMap m)
        {
            auto is_pair = [](auto xx)
            {
                return pair::valid(xx);
            };

            return list::all_match(m, is_pair);
        }
    }

    /// @brief Returns true if `m` is a valid map.
    template <typename TMap>
    constexpr auto valid(TMap m)
    {
        return decltype(impl::valid_impl(m)){};
    }

    /// @brief Returns the number of key-value pairs in `m`.
    template <typename TMap>
    constexpr auto size(TMap m)
    {
        return list::size(m);
    }

    /// @brief Returns a copy of `m`, with `p` inserted in it.
    template <typename TMap, typename TPair>
    constexpr auto insert(TMap m, TPair p)
    {
        (void)m;
        (void)p;

        ECST_S_ASSERT_DT(pair::valid(p));
        auto res(list::append(TMap{}, TPair{}));

        ECST_S_ASSERT_DT(valid(res));
        return res;
    }

    /// @brief Returns a copy of `m`, with `(x0, x1)` inserted in it.
    template <typename TMap, typename T0, typename T1>
    constexpr auto emplace(TMap m, T0 x0, T1 x1)
    {
        return insert(m, pair::make(x0, x1));
    }

    // TODO: set/emplace_or_replace

    /// @brief Returns the key of a key-value pair.
    template <typename TPair>
    constexpr auto key(TPair p)
    {
        return pair::fst(p);
    }

    /// @brief Returns the value of a key-value pair.
    template <typename TPair>
    constexpr auto value(TPair p)
    {
        return pair::snd(p);
    }
}
ECST_MP_MAP_NAMESPACE_END
