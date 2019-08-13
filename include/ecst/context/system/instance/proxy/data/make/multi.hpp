// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../impl.hpp"

namespace ecst::context::system
{
    namespace data_proxy
    {
        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TInstance,        // .
            typename... Ts             // .
            >
        auto ECST_PURE_FN make_multi(
            TInstance& inst, TContext& ctx, Ts&&... xs) noexcept
        {
            return multi<TSystemSignature, TContext, TInstance>(
                inst, ctx, FWD(xs)...);
        }
    } // namespace data_proxy
} // namespace ecst::context::system
