// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::inner_par::strategy::none::impl
{

    template <typename TParameters>
    struct executor
    {
        template <typename TInstance, typename TContext, typename TF>
        void execute(TInstance& inst, TContext&, TF&& f)
        {
            // Clean/allocate space for a state and an eventual output.
            inst.prepare_single_subtask();

            // Immediately execute `f` in the zero-th state, with a
            // range of entities equal to `[0, subscribed_count)`.
            f(0, 0, inst.subscribed_count());
        }
    };
} // namespace ecst::inner_par::strategy::none::impl
