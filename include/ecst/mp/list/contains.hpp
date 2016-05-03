// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/basic.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TL>
        struct set
        {
        };

        template <typename... Ts>
        struct set<type_list<Ts...>> : Ts...
        {
            using type = set;
        };

        template <typename TSet, typename T>
        struct in_set
        {
        };

        template <typename... Ts, typename T>
        struct in_set<type_list<Ts...>, T>
            : std::is_base_of<T, set<type_list<Ts...>>>
        {
        };
    }

    /// @brief Returns true if any element in `l` is equal to `T`.
    template <typename TList, typename T>
    constexpr auto contains(TList, T)
    {
        return bool_v<(impl::in_set<TList, T>::value)>;
    }
}
ECST_MP_LIST_NAMESPACE_END
