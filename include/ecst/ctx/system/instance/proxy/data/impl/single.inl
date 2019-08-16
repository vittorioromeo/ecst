// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./single.hpp"

#define ECST_IMPL_DP_SINGLE_TEMPLATE \
    template <typename TSystemSignature, typename TContext, typename TInstance>

#define ECST_IMPL_DP_SINGLE single<TSystemSignature, TContext, TInstance>

namespace ecst::ctx::system::data_proxy
{
    ECST_IMPL_DP_SINGLE_TEMPLATE
    auto& ECST_IMPL_DP_SINGLE::subtask_state_wrapper() noexcept
    {
        return this->_instance.nth_state(0);
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    template <typename TF>
    auto ECST_IMPL_DP_SINGLE::for_entities(TF&& f)
    {
        return this->_instance.for_all_entities(FWD(f));
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    template <typename TF>
    constexpr auto ECST_IMPL_DP_SINGLE::for_other_entities(TF&&) noexcept
    {
        return ecst::impl::do_nothing;
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    template <typename TF>
    auto ECST_IMPL_DP_SINGLE::for_all_entities(TF&& f)
    {
        return for_entities(FWD(f));
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    auto ECST_IMPL_DP_SINGLE::entity_count() const noexcept
    {
        return this->_instance.subscribed_count();
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    auto ECST_IMPL_DP_SINGLE::all_entity_count() const noexcept
    {
        return entity_count();
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    constexpr auto ECST_IMPL_DP_SINGLE::other_entity_count() const noexcept
    {
        return sz_t(0);
    }

    ECST_IMPL_DP_SINGLE_TEMPLATE
    constexpr auto ECST_IMPL_DP_SINGLE::subtask_index() const noexcept
    {
        return sz_t(0);
    }
} // namespace ecst::ctx::system::data_proxy

#undef ECST_IMPL_DP_SINGLE
#undef ECST_IMPL_DP_SINGLE_TEMPLATE
