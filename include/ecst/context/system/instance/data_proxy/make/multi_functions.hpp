// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../impl.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        template <                       // .
            typename TFForEntities,      // .
            typename TFForAllEntities,   // .
            typename TFForOtherEntities, // .
            typename TFStateGetter       // .
            >
        auto ECST_PURE_FN make_multi_functions(        // .
            TFForEntities&& f_for_entities,            // .
            TFForAllEntities&& f_for_all_entities,     // .
            TFForOtherEntities&& f_for_other_entities, // .
            TFStateGetter&& f_state_getter             // .
            ) noexcept
        {
            struct multi_functions
            {
                TFForEntities _f_for_entities;
                TFForAllEntities _f_for_all_entities;
                TFForOtherEntities _f_for_other_entities;
                TFStateGetter _f_state_getter;
            };

            return multi_functions{
                FWD(f_for_entities),       // .
                FWD(f_for_all_entities),   // .
                FWD(f_for_other_entities), // .
                FWD(f_state_getter)        // .
            };
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
