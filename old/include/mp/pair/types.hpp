// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>

ECST_MP_PAIR_NAMESPACE
{
    namespace impl
    {
        // A `type_pair` is an `mp::list`.
        template <typename T0, typename T1>
        using type_pair = ecst::mp::list::impl::type_list<T0, T1>;
    }

    // Variable template for `type_pair`.
    template <typename T0, typename T1>
    constexpr impl::type_pair<T0, T1> v{};
}
ECST_MP_PAIR_NAMESPACE_END
