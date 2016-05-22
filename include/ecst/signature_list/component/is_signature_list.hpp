// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/component.hpp>

ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE
{
    namespace impl
    {
        template <typename T>
        auto valid_impl()
        {
            return mp::bh::all_of(T{}, [](auto sx)
                {
                    return signature::component::valid(sx);
                });
        }
    }

    template <typename T>
    constexpr auto valid(T)
    {
        return decltype(impl::valid_impl<T>()){};
    }
}
ECST_SIGNATURE_LIST_COMPONENT_NAMESPACE_END
