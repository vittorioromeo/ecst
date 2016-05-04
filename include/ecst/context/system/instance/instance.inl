// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/hardware.hpp>
#include "./instance.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    template <typename TSettings, typename TSystemSignature>
    instance<TSettings, TSystemSignature>::instance()
        : _bitset{
              bitset::make_from_system_signature<TSystemSignature>(TSettings{})}
    {
        ELOG(                                                          // .
            debug::lo_system_bitset() << "(" << system_id()            // .
                                      << ") bitset: " << _bitset.str() // .
                                      << "\n";                         // .
            );
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    decltype(auto) instance<TSettings, TSystemSignature>::for_states(TF && f)
    {
        return _sm.for_states(FWD(f));
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TProxy>
    void instance<TSettings, TSystemSignature>::execute_deferred_fns(
        TProxy & proxy)
    {
        for_states([&proxy](auto& state)
            {
                state._state.execute_deferred_fns(proxy);
            });
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    decltype(auto) instance<TSettings, TSystemSignature>::for_outputs(TF && f)
    {
        return for_states([this, &f](auto& state)
            {
                f(_system, state.as_data());
            });
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TAcc, typename TF>
    auto instance<TSettings, TSystemSignature>::foldl_outputs(TAcc acc, TF && f)
    {
        for_outputs([&acc, xf = FWD(f) ](auto& system, auto& state_data)
            {
                xf(system, acc, state_data);
            });

        return acc;
    }


    template <typename TSettings, typename TSystemSignature>
    auto instance<TSettings, TSystemSignature>::is_subscribed(entity_id eid)
        const noexcept
    {
        return subscribed().has(eid);
    }

    template <typename TSettings, typename TSystemSignature>
    auto instance<TSettings, TSystemSignature>::subscribe(entity_id eid)
    {
        // TODO: callback, optional subscription function that returns bool to
        // ignore
        return subscribed().add(eid);
    }

    template <typename TSettings, typename TSystemSignature>
    auto instance<TSettings, TSystemSignature>::unsubscribe(entity_id eid)
    {
        // TODO: callback, optional subscription function that returns bool to
        // ignore
        return subscribed().erase(eid);
    }

    template <typename TSettings, typename TSystemSignature>
    template <                          // .
        typename TFEntityProvider,      // .
        typename TFAllEntityProvider,   // .
        typename TFOtherEntityProvider, // .
        typename TContext,              // .
        typename TFStateGetter          // .
        >
    auto instance<TSettings, TSystemSignature>::make_data( // .
        TFEntityProvider && f_ep,                          // .
        sz_t ep_count,                                     // .
                                                           // .
        TFAllEntityProvider && f_aep,                      // .
        sz_t ae_count,                                     // .
                                                           // .
        TFOtherEntityProvider && f_oep,                    // .
        sz_t oe_count,                                     // .
                                                           // .
        TContext & ctx,                                    // .
        TFStateGetter && sg                                // .
        )
    {
        // `mutable` to allow implementations to be `mutable` as well.
        auto make_entity_id_adapter = [](auto&& f) mutable
        {
            return [f = FWD(f)](auto&& g) mutable
            {
                return f([g = FWD(g)](auto id) mutable
                    {
                        return g(entity_id(id));
                    });
            };
        };

        return impl::make_execute_data<TSystemSignature> // .
            (                                            // .
                ctx,                                     // .
                                                         // .
                make_entity_id_adapter(FWD(f_ep)),       // .
                ep_count,                                // .
                                                         // .
                make_entity_id_adapter(FWD(f_aep)),      // .
                ae_count,                                // .
                                                         // .
                make_entity_id_adapter(FWD(f_oep)),      // .
                oe_count,                                // .
                                                         // .
                FWD(sg)                                  // .
                );
    }


    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    void instance<TSettings, TSystemSignature>::prepare_and_wait_n_subtasks(
        sz_t n, TF && f)
    {
        _sm.clear_and_prepare(n);
        counter_blocker b{n};

        // Runs the parallel executor and waits until the remaining subtasks
        // counter is zero.
        execute_and_wait_until_counter_zero(b, [this, &b, &f]
            {
                f(b);
            });
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TCounterBlocker, typename TData, typename TF>
    void instance<TSettings,
        TSystemSignature>::execute_subtask_and_decrement_counter( // .
        TCounterBlocker & cb, TData & data, TF && f               // .
        )
    {
        f(data);
        decrement_cv_counter_and_notify_all(cb);
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::execute_single( // .
        TContext & ctx, TF && f                                 // .
        )
    {
        _sm.clear_and_prepare(1);

        // TODO: refactor/create `single_execute_data`.
        auto data = make_data(          // .
            make_all_entity_provider(), // .
            all_entity_count(),         // .
                                        // .
            make_all_entity_provider(), // .
            all_entity_count(),         // .
                                        // .
            [](auto&&...)               // .
            {                           // .
            },                          // .
            0,                          // .
                                        // .
            ctx,
            [this]() -> auto&
            {
                return this->_sm.get(0);
            });

        f(data);
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::execute_in_parallel( // .
        TContext & ctx, TF && f                                      // .
        )
    {
        // Aggregates synchronization primitives.
        _parallel_executor.execute(*this, ctx, f);
    }

    // TODO: docs/refactor
    template <typename TInstance, typename TFExecution>
    struct executor_helper
    {
        TInstance& _instance;
        TFExecution _execution;

        template <typename TFwdFExecution>
        executor_helper(TInstance& instance,
            TFwdFExecution&& execution) noexcept : _instance{instance},
                                                   _execution(FWD(execution))
        {
        }

        template <typename TF>
        auto for_subtasks(TF&& f) noexcept
        {
            return _execution(f);
        }

        auto& instance() noexcept
        {
            return _instance;
        }

        const auto& instance() const noexcept
        {
            return _instance;
        }

        auto& system() noexcept
        {
            return _instance.system();
        }

        const auto& system() const noexcept
        {
            return _instance.system();
        }
    };

    // TODO: docs/refactor
    template <typename TInstance, typename... TFs>
    auto make_executor_helper(TInstance & instance, TFs && ... fs) noexcept
    {
        return executor_helper<TInstance, std::decay_t<TFs>...>{
            instance, FWD(fs)...};
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::execute( // .
        TContext & ctx, TF && f                          // .
        )
    {
        auto execution = [this, &ctx](auto&& sb_f)
        {
            return static_if(settings::inner_parallelism_allowed<TSettings>())
                .then([this, &sb_f](auto& xsp)
                    {
                        return this->execute_in_parallel(xsp, sb_f);
                    })
                .else_([this, &sb_f](auto& xsp)
                    {
                        return this->execute_single(xsp, sb_f);
                    })(ctx);
        };

        // TODO: docs/refactor
        auto eh = make_executor_helper(*this, std::move(execution));
        f(_system, eh);
    }

    template <typename TSettings, typename TSystemSignature>
    auto& instance<TSettings, TSystemSignature>::system() noexcept
    {
        return _system;
    }

    template <typename TSettings, typename TSystemSignature>
    const auto& instance<TSettings, TSystemSignature>::system() const noexcept
    {
        return _system;
    }

    template <typename TSettings, typename TSystemSignature>
    const auto& instance<TSettings, TSystemSignature>::bitset() const noexcept
    {
        return _bitset;
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TBitset>
    auto instance<TSettings, TSystemSignature>::matches_bitset(const TBitset& b)
        const noexcept
    {
        return _bitset.contains(b);
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::subscribed_count()
        const noexcept
    {
        return this->subscribed().size();
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
