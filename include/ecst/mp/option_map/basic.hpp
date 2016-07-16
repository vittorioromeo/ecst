// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list.hpp>
#include "./replace.hpp"

ECST_MP_OPTION_MAP_NAMESPACE
{
    namespace impl
    {
        template <typename TMap>
        class data
        {
        private:
            TMap _map;

        public:
            template <typename TKey>
            constexpr auto type_at(const TKey& key) const noexcept
            {
                return type_c<ECST_DECAY_DECLTYPE(
                    bh::first(bh::at_key(_map, key)))>;
            }

            template <typename TKey>
            constexpr auto at(const TKey& key) const noexcept
            {
                return bh::first(bh::at_key(_map, key));
            }

            template <typename TKey, typename T>
            constexpr auto add(const TKey& key, T&& x) const noexcept
            {
                auto new_map = bh::insert(_map,
                    bh::make_pair(key, bh::make_pair(FWD(x), bh::false_c)));

                return data<ECST_DECAY_DECLTYPE(new_map)>{};
            }

            template <typename TKey, typename T>
            constexpr auto set(const TKey& key, T&& x) const noexcept
            {
                // Prevent setting same setting twice.
                ECST_S_ASSERT_DT(
                    bh::second(bh::at_key(_map, key)) == bh::false_c);

                auto new_map = impl::replace(_map,
                    bh::make_pair(key, bh::make_pair(FWD(x), bh::true_c)));

                return data<ECST_DECAY_DECLTYPE(new_map)>{};
            }
        };
    }

    constexpr auto make() noexcept
    {
        return impl::data<ECST_DECAY_DECLTYPE(bh::make_map())>{};
    }

    /// @brief Returns the type of the value stored in `TMap` at key `TKey`.
    template <typename TMap, typename TKey>
    using type_of = unwrap<decltype(TMap{}.type_at(TKey{}))>;
}
ECST_MP_OPTION_MAP_NAMESPACE_END
