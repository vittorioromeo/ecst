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
        return impl::data<TSettings>{};
    }

    template <typename TSettings>
    auto make_uptr(TSettings)
    {
        return std::make_unique<decltype(make(TSettings{}))>(make(TSettings{}));
    }
}
ECST_CONTEXT_NAMESPACE_END
