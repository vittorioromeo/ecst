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
        template <                  // .
            typename TFForEntities, // .
            typename TFStateGetter  // .
            >
        auto ECST_PURE_FN make_single_execute_data_functions( // .
            TFForEntities&& f_for_entities,                   // .
            TFStateGetter&& f_state_getter                    // .
            ) noexcept
        {
            struct single_execute_data_functions
            {
                TFForEntities _f_for_entities;
                TFStateGetter _f_state_getter;
            };

            return single_execute_data_functions{
                FWD(f_for_entities), // .
                FWD(f_state_getter)  // .
            };
        }

        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TEDFunctions,     // .
            typename... Ts             // .
            >
        auto ECST_PURE_FN make_single_execute_data( // .
            TContext& ctx, TEDFunctions&& functions, Ts&&... xs)
        {
            return single_execute_data<TSystemSignature, TContext,
                TEDFunctions>(ctx, std::move(functions), FWD(xs)...);
        }

        template <                       // .
            typename TFForEntities,      // .
            typename TFForAllEntities,   // .
            typename TFForOtherEntities, // .
            typename TFStateGetter       // .
            >
        auto ECST_PURE_FN make_execute_data_functions( // .
            TFForEntities&& f_for_entities,            // .
            TFForAllEntities&& f_for_all_entities,     // .
            TFForOtherEntities&& f_for_other_entities, // .
            TFStateGetter&& f_state_getter             // .
            ) noexcept
        {
            struct execute_data_functions
            {
                TFForEntities _f_for_entities;
                TFForAllEntities _f_for_all_entities;
                TFForOtherEntities _f_for_other_entities;
                TFStateGetter _f_state_getter;
            };

            return execute_data_functions{
                FWD(f_for_entities),       // .
                FWD(f_for_all_entities),   // .
                FWD(f_for_other_entities), // .
                FWD(f_state_getter)        // .
            };
        }

        template <                     // .
            typename TSystemSignature, // .
            typename TContext,         // .
            typename TEDFunctions,     // .
            typename... Ts             // .
            >
        auto ECST_PURE_FN make_execute_data( // .
            TContext& ctx, TEDFunctions&& functions, Ts&&... xs)
        {
            return execute_data<TSystemSignature, TContext, TEDFunctions>(
                ctx, std::move(functions), FWD(xs)...);
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
