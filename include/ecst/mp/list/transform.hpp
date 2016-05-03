// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list/fold.hpp>

ECST_MP_LIST_NAMESPACE
{
    namespace impl
    {
        template <typename TList, typename TF>
        constexpr auto transform_impl(TF f, TList l)
        {
            return fold_l(empty_v,
                [=](auto acc, auto x_item)
                {
                    return append(acc, f(x_item));
                },
                l);
        }
    }

    template <typename TList, typename TF>
    constexpr auto transform(TF f, TList l)
    {
        return decltype(impl::transform_impl(f, l)){};
    }
}
ECST_MP_LIST_NAMESPACE_END
