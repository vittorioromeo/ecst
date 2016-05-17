// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"

ECST_CONTEXT_NAMESPACE
{
    template <typename TSettings>
    auto make(TSettings)
    {
        // TODO: ?
        return impl::data<TSettings>{};

        /*
        auto result = impl::data<TSettings>{};
        using rt = decltype(result);

        struct e : rt
        {
            using rt::rt;
            e(rt&& x) : rt{std::move(x)}
            {
            }
        };

        auto i = e{std::move(result)};

        return i;
        */
    }

    template <typename TSettings>
    auto make_uptr(TSettings)
    {
        return std::make_unique<decltype(make(TSettings{}))>(make(TSettings{}));
    }
}
ECST_CONTEXT_NAMESPACE_END
