// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list.hpp>

ECST_MP_OPTION_MAP_NAMESPACE
{
    template <typename TMap>
    struct option_map_impl
    {
        constexpr auto map()
        {
            return TMap{};
        }

        template <typename TKey, typename T>
        constexpr auto add(const TKey& key, T&& x) noexcept
        {
            auto new_map = bh::insert(
                map(), bh::make_pair(key, bh::make_pair(FWD(x), bh::false_c)));

            return option_map_impl<std::decay_t<decltype(new_map)>>{};
        }

        template <typename TKey, typename T>
        constexpr auto set(const TKey& key, T&& x) noexcept
        {
            // Prevent setting same setting twice.
            ECST_S_ASSERT_DT(bh::second(bh::at_key(map(), key)) == bh::false_c);

            auto new_map = bh::insert(bh::erase_key(map(), key),
                bh::make_pair(key, bh::make_pair(FWD(x), bh::true_c)));

            return option_map_impl<std::decay_t<decltype(new_map)>>{};
        }
    };

    constexpr auto make() noexcept
    {
        return option_map_impl<std::decay_t<decltype(bh::make_map())>>{};
    }

    // TODO: docs, optimize
    template <typename TMap, typename TKey>
    using type_of = std::decay_t<decltype(
        mp::bh::first(mp::bh::at_key(TMap{}.map(), TKey{})))>;
}
ECST_MP_OPTION_MAP_NAMESPACE_END
