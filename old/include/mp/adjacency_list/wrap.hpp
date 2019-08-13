// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/map.hpp>
#include <ecst/mp/adjacency_list/types.hpp>
#include <ecst/mp/adjacency_list/is_adjacency_list.hpp>

ECST_MP_ADJACENCY_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TWrappedAList>
        constexpr auto unwrap(TWrappedAList wal)
        {
            return wal;
        }

        template <typename TUnwrappedAList>
        constexpr auto wrap(TUnwrappedAList uwal)
        {
            return uwal;
        }
    }
}

