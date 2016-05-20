// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <tuple>
#include <utility>
#include <ecst/config.hpp>
#include <ecst/mp/core.hpp>
#include <ecst/mp/list/types.hpp>

ECST_MP_NAMESPACE
{
    namespace impl
    {
#if defined(VRM_CORE_COMPILER_GCC)
        template <typename T>
        using is_list_impl =
            is_specialization_of<mp::list::impl::type_list, std::decay_t<T>>;
#else
        template <typename T>
        using is_list_impl = bh::true_;
#endif
    }

    template <typename T>
    constexpr auto is_list(T)
    {
        return impl::is_list_impl<T>{};
    }
}
ECST_MP_NAMESPACE_END
