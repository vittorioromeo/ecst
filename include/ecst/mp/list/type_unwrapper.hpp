// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>
#include <ecst/mp/list/inverse_indices.hpp>
#include <ecst/mp/list/remove.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        struct list_unwrapper;

        template <typename... Ts>
        struct list_unwrapper<type_list<Ts...>>
        {
            using type = type_list<typename Ts::type...>;
        };

        template <typename T>
        using unwrap_type_v_list = typename list_unwrapper<T>::type;
    }

    template <typename T>
    using unwrap_tuple = impl::unwrap_type_v_list<T>;
}
ECST_MP_LIST_NAMESPACE_END
