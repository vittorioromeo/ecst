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
        template <                  // .
            typename TFForEntities, // .
            typename TFStateGetter  // .
            >
        auto ECST_PURE_FN make_single_functions( // .
            TFForEntities&& f_for_entities,      // .
            TFStateGetter&& f_state_getter       // .
            ) noexcept
        {
            struct single_functions
            {
                TFForEntities _f_for_entities;
                TFStateGetter _f_state_getter;
            };

            return single_functions{
                FWD(f_for_entities), // .
                FWD(f_state_getter)  // .
            };
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
