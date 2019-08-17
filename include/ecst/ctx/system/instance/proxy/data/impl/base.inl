// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./base.hpp"

#define ECST_IMPL_DP_BASE_TEMPLATE                          \
    template <typename TSystemSignature, typename TContext, \
        typename TInstance, typename TDerived>

#define ECST_IMPL_DP_BASE base<TSystemSignature, TContext, TInstance, TDerived>

namespace ecst::ctx::system::data_proxy
{
    ECST_IMPL_DP_BASE_TEMPLATE
    auto& ECST_IMPL_DP_BASE::subtask_state_wrapper() noexcept
    {
        // CRTP is used to efficiently get the state index.
        return vrmc::to_derived<TDerived>(*this).subtask_state_wrapper();
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    auto& ECST_IMPL_DP_BASE::subtask_state() noexcept
    {
        return subtask_state_wrapper().as_state();
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    auto& ECST_IMPL_DP_BASE::subtask_state_data() noexcept
    {
        return subtask_state_wrapper().as_data();
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    template <typename TSystemTag>
    constexpr auto ECST_IMPL_DP_BASE::can_get_output_of(TSystemTag st) noexcept
    {
        constexpr auto ssl = settings::system_sig_list(settings_type{});

        constexpr auto my_ss = TSystemSignature{};

        constexpr auto target_ss = sig_list::system::sig_by_tag(ssl, st);

        return sig_list::system::has_dependency_recursive(
            ssl, my_ss, target_ss);
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    ECST_IMPL_DP_BASE::base(     // .
        instance_type& instance, // .
        context_type& context    // .
        ) noexcept
        : _instance{instance}, // .
          _context{context}    // .
    {
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    template <typename TComponentTag>
    decltype(auto) ECST_IMPL_DP_BASE::get(
        TComponentTag ct, entity_id eid) noexcept
    {
        if constexpr(sig::system::can_write<TSystemSignature>(ct))
        {
            return _context.get_component(ct, eid);
        }
        else if constexpr(sig::system::can_read<TSystemSignature>(ct))
        {
            return _context.get_component(ct, eid);
        }
        else
        {
            // TODO: nicer error message
            struct cant_access_that_component;
            return cant_access_that_component{};
        }
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    template <typename TComponentTag>
    auto ECST_IMPL_DP_BASE::has(TComponentTag ct, entity_id eid) const noexcept
    {
        return _context.has_component(ct, eid);
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    template <typename TF>
    void ECST_IMPL_DP_BASE::defer(TF&& f)
    {
        subtask_state()._deferred_fns.add(FWD(f));
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    void ECST_IMPL_DP_BASE::kill_entity(entity_id eid)
    {
        subtask_state()._to_kill.add(eid);
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    auto& ECST_IMPL_DP_BASE::output() noexcept
    {
        static_assert(sig::system::has_subtask_state_data<system_sig_type>());

        return subtask_state_data();
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    template <typename TSystemTag>
    auto& ECST_IMPL_DP_BASE::system(TSystemTag st) noexcept
    {
        return _context.system(st);
    }

    ECST_IMPL_DP_BASE_TEMPLATE
    template <typename TSystemTag, typename TF>
    decltype(auto) ECST_IMPL_DP_BASE::for_previous_outputs(
        TSystemTag st, TF&& f) noexcept
    {
        ECST_S_ASSERT_DT(can_get_output_of(st));
        return _context.for_system_outputs(st, FWD(f));
    }
} // namespace ecst::ctx::system::data_proxy


#undef ECST_IMPL_DP_BASE
#undef ECST_IMPL_DP_BASE_TEMPLATE
