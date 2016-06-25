// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp.hpp>

ECST_INNER_PARALLELISM_NAMESPACE
{
    namespace utils
    {
        template <typename TFRIST, typename TContext, typename TF>
        void execute_split_runtime(                       // .
            sz_t total, sz_t per_split, sz_t split_count, // .
            TFRIST& rist, TContext& ctx, TF&& f)          // .
            noexcept(noexcept(                            // .
                f(0, 0, 0)                                // .
                ))
        {
            ECST_ASSERT(split_count > 0);

            for(sz_t i_split = 0; i_split < split_count - 1; ++i_split)
            {
                auto i_begin = i_split * per_split;
                auto i_end = (i_split + 1) * per_split;
                rist(ctx, f)(i_split, i_begin, i_end);
            }

            // Builds and runs the last subtask.
            {
                auto i_split = split_count - 1;
                auto i_begin = i_split * per_split;
                auto i_end = total;
                f(i_split, i_begin, i_end);
            }
        }
    }
}
ECST_INNER_PARALLELISM_NAMESPACE_END
