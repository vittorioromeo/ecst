// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./data.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <                       // .
            typename TSystemSignature,   // .
            typename TContext,           // .
            typename TFForEntities,      // .
            typename TFForAllEntities,   // .
            typename TFForOtherEntities, // .
            typename TFStateGetter       // .
            >
        auto ECST_PURE_FN make_execute_data(           // .
            TContext& context,                         // .
            TFForEntities&& f_for_entities,            // .
            TFForAllEntities&& f_for_all_entities,     // .
            TFForOtherEntities&& f_for_other_entities, // .
            TFStateGetter&& f_state_getter             // .
            )
        {
            return execute_data<                  // .
                TSystemSignature,                 // .
                TContext,                         // .
                std::decay_t<TFForEntities>,      // .
                std::decay_t<TFForAllEntities>,   // .
                std::decay_t<TFForOtherEntities>, // .
                std::decay_t<TFStateGetter>       // .
                >                                 // .
                {
                    context,                   // .
                    FWD(f_for_entities),       // .
                    FWD(f_for_all_entities),   // .
                    FWD(f_for_other_entities), // .
                    FWD(f_state_getter)        // .
                };
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
