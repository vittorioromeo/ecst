// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>

ECST_MP_MAP_NAMESPACE
{
    namespace impl
    {
        // A `type_map` is an `mp::list`.
        template <typename... Ts>
        using type_map = ecst::mp::list::impl::type_list<Ts...>;

        // An empty map is an empty `type_map`.
        using empty_type_map = type_map<>;
    }

    // Variable template for `type_map`.
    template <typename... Ts>
    constexpr impl::type_map<Ts...> v{};

    // Variable template for an empty list.
    constexpr impl::empty_type_map empty_v{};
}
ECST_MP_MAP_NAMESPACE_END
