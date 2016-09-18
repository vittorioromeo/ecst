// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./multi.hpp"

#define ECST_IMPL_DP_MULTI_TEMPLATE \
    template <typename TSystemSignature, typename TContext, typename TInstance>

#define ECST_IMPL_DP_MULTI multi<TSystemSignature, TContext, TInstance>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        ECST_IMPL_DP_MULTI_TEMPLATE
        ECST_IMPL_DP_MULTI::multi(TInstance& instance, TContext& context,
            sz_t state_idx, sz_t i_begin, sz_t i_end) noexcept
            : base_type{instance, context},
              _state_idx{state_idx},
              _i_begin{i_begin},
              _i_end{i_end}
        {
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        auto& ECST_IMPL_DP_MULTI::subtask_state_wrapper() noexcept
        {
            return this->_instance.nth_state(_state_idx);
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_DP_MULTI::for_entities(TF&& f)
        {
            return this->_instance.for_entities(_i_begin, _i_end, FWD(f));
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_DP_MULTI::for_other_entities(TF&& f)
        {
            return this->_instance.for_other_entities(_i_begin, _i_end, FWD(f));
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_DP_MULTI::for_all_entities(TF&& f)
        {
            return this->_instance.for_all_entities(FWD(f));
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        auto ECST_IMPL_DP_MULTI::entity_count() const noexcept
        {
            return this->_instance.entity_range_count(_i_begin, _i_end);
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        auto ECST_IMPL_DP_MULTI::all_entity_count() const noexcept
        {
            return this->_instance.all_entity_count();
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        auto ECST_IMPL_DP_MULTI::other_entity_count() const noexcept
        {
            return this->_instance.other_entity_range_count(_i_begin, _i_end);
        }

        ECST_IMPL_DP_MULTI_TEMPLATE
        auto ECST_IMPL_DP_MULTI::subtask_index() const noexcept
        {
            return _state_idx;
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_DP_MULTI
#undef ECST_IMPL_DP_MULTI_TEMPLATE
