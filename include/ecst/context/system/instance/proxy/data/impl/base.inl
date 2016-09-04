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

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        ECST_IMPL_DP_BASE_TEMPLATE
        auto& ECST_IMPL_DP_BASE::state_wrapper() noexcept
        {
            // CRTP is used to efficiently get the state index.
            return vrmc::to_derived<TDerived>(*this).state_wrapper();
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        auto& ECST_IMPL_DP_BASE::state() noexcept
        {
            return state_wrapper().as_state();
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        auto& ECST_IMPL_DP_BASE::output_data() noexcept
        {
            return state_wrapper().as_data();
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        template <typename TSystemTag>
        constexpr auto ECST_IMPL_DP_BASE::can_get_output_of(
            TSystemTag st) noexcept
        {
            constexpr auto ssl =
                settings::system_signature_list(settings_type{});

            constexpr auto my_ss = TSystemSignature{};

            constexpr auto target_ss =
                signature_list::system::signature_by_tag(ssl, st);

            return signature_list::system::has_dependency_recursive(
                ssl, my_ss, target_ss);
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        ECST_IMPL_DP_BASE::base(              // .
            instance_type& instance,          // .
            context_type& context             // .
            ) noexcept : _instance{instance}, // .
                         _context{context}    // .
        {
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        template <typename TComponentTag>
        decltype(auto) ECST_IMPL_DP_BASE::get(
            TComponentTag ct, entity_id eid) noexcept
        {
            constexpr auto can_write =
                signature::system::can_write<TSystemSignature>(ct);

            constexpr auto can_read =
                signature::system::can_read<TSystemSignature>(ct);

            return static_if(can_write)
                .then([ ct, eid ](auto& x_ctx) -> auto&
                    {
                        return x_ctx.get_component(ct, eid);
                    })
                .else_if(can_read)
                .then([ ct, eid ](auto& x_ctx) -> const auto&
                    {
                        return x_ctx.get_component(ct, eid);
                    })
                .else_([](auto&)
                    {
                        // TODO: nicer error message
                        struct cant_access_that_component;
                        return cant_access_that_component{};
                    })(_context);
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        template <typename TF>
        void ECST_IMPL_DP_BASE::defer(TF&& f)
        {
            state()._deferred_fns.add(FWD(f));
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        void ECST_IMPL_DP_BASE::kill_entity(entity_id eid)
        {
            state()._to_kill.add(eid);
        }

        ECST_IMPL_DP_BASE_TEMPLATE
        auto& ECST_IMPL_DP_BASE::output() noexcept
        {
            ECST_S_ASSERT(
                signature::system::has_data_output<system_signature_type>());

            return output_data();
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
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_DP_BASE
#undef ECST_IMPL_DP_BASE_TEMPLATE
