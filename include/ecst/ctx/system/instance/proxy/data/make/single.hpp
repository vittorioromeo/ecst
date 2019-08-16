// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../impl.hpp"

namespace ecst::ctx::system::data_proxy
{
    template <                     // .
        typename TSystemSignature, // .
        typename TContext,         // .
        typename TInstance,        // .
        typename... Ts             // .
        >
    auto ECST_PURE_FN make_single(
        TInstance& inst, TContext& ctx, Ts&&... xs) noexcept
    {
        return single<TSystemSignature, TContext, TInstance>(
            inst, ctx, FWD(xs)...);
    }
} // namespace ecst::ctx::system::data_proxy
