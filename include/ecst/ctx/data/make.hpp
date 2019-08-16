// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"

namespace ecst::ctx
{
    template <typename TSettings>
    auto make(TSettings)
    {
        return impl::data<TSettings>{};
    }

    template <typename TSettings>
    auto make_uptr(TSettings)
    {
        using ctx_type = decltype(make(TSettings{}));
        return std::make_unique<ctx_type>();
    }
} // namespace ecst::ctx
