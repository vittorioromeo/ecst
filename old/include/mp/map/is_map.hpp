// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/mp/map/types.hpp>

namespace ecst::mp
{
    namespace impl
    {
#if defined(VRM_CORE_COMPILER_GCC)
        template <typename T>
        using is_map_impl =
            is_specialization_of_t<std::decay_t<T>, mp::map::impl::type_map>;
#else
        template <typename T>
        using is_map_impl = bool_<true>;
#endif
    }

    template <typename T>
    constexpr auto is_map(T)
    {
        return bool_v<impl::is_map_impl<T>{}>;
    }
}

