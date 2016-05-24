// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/mp/list.hpp>

ECST_MP_OPTION_MAP_NAMESPACE
{
    // TODO:
    namespace impl
    {
        template <typename NewPair>
        struct replace_helper_t
        {
            NewPair const& new_pair;

            template <typename Pair>
            constexpr decltype(auto) operator()(Pair&& p) const
            {
                return bh::if_(
                    bh::hash(bh::first(new_pair)) == bh::hash(bh::first(p)),
                    new_pair, FWD(p));
            }
        };

        struct replace_t
        {
            template <typename Map, typename NewPair>
            constexpr auto operator()(Map&& m, NewPair&& new_pair) const
            {
                return bh::unpack(std::forward<Map>(m),
                    bh::on(bh::make_map, replace_helper_t<NewPair>{
                                             FWD(new_pair)}));
            }
        };

        constexpr replace_t replace{};
    }

    template <typename TMap>
    class option_map_impl
    {
    private:
        TMap _map;

    public:
        template <typename TKey>
        constexpr auto type_at(const TKey& key) noexcept
        {
            return bh::type_c<
                std::decay_t<decltype(bh::first(bh::at_key(_map, key)))>>;
        }

        template <typename TKey, typename T>
        constexpr auto add(const TKey& key, T&& x) noexcept
        {
            auto new_map = bh::insert(
                _map, bh::make_pair(key, bh::make_pair(FWD(x), bh::false_c)));

            return option_map_impl<std::decay_t<decltype(new_map)>>{};
        }

        template <typename TKey, typename T>
        constexpr auto set(const TKey& key, T&& x) noexcept
        {
            // Prevent setting same setting twice.
            ECST_S_ASSERT_DT(bh::second(bh::at_key(_map, key)) == bh::false_c);

            auto new_map = impl::replace(
                _map, bh::make_pair(key, bh::make_pair(FWD(x), bh::true_c)));

            return option_map_impl<std::decay_t<decltype(new_map)>>{};
        }
    };

    constexpr auto make() noexcept
    {
        return option_map_impl<std::decay_t<decltype(bh::make_map())>>{};
    }

    // TODO: docs, optimize
    template <typename TMap, typename TKey>
    using type_of = unwrap<decltype(TMap{}.type_at(TKey{}))>;
}
ECST_MP_OPTION_MAP_NAMESPACE_END
