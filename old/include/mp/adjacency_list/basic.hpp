// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/map.hpp>
#include <ecst/mp/adjacency_list/wrap.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    /// @brief Returns an empty adjacency list.
    /// @details The keys of the map are nodes, the values are lists of pairs.
    /// The pairs inside the lists have the goal node as their first element and
    /// the data of the edge as their second element.
    constexpr auto make()
    {
        return impl::wrap(map::make());
    }

    namespace impl
    {
        template <typename TAList>
        constexpr auto valid_impl(TAList al)
        {
            return map::all_values_matching(impl::unwrap(al), [](auto v)
                {
                    return static_and(
                        list::valid(v), list::all_match(v, [](auto xv)
                                            {
                                                return pair::valid(xv);
                                            }));
                });
        }
    }

    /// @brief Returns true if `al` is a valid adjacency list.
    template <typename TAList>
    constexpr auto valid(TAList al)
    {
        return decltype(impl::valid_impl(al)){};
    }
}

