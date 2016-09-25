// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./instance.hpp"
#include "./proxy.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    template <typename TSettings, typename TSystemSignature>
    ECST_ALWAYS_INLINE auto& ECST_PURE_FN
    instance<TSettings, TSystemSignature>::subscribed() noexcept
    {
        return _subscribed;
    }

    template <typename TSettings, typename TSystemSignature>
    ECST_ALWAYS_INLINE const auto& ECST_PURE_FN
    instance<TSettings, TSystemSignature>::subscribed() const noexcept
    {
        return _subscribed;
    }

    template <typename TSettings, typename TSystemSignature>
    instance<TSettings, TSystemSignature>::instance()
        : _bitset{bitset::make_from_system_signature(
              TSystemSignature{}, TSettings{})}
    {
        ELOG(                                                          // .
            debug::lo_system_bitset() << "(" << system_id()            // .
                                      << ") bitset: " << _bitset.str() // .
                                      << "\n";                         // .
            );
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TProxy>
    void instance<TSettings, TSystemSignature>::execute_deferred_fns(
        TProxy & proxy)
    {
        this->for_states([&proxy](auto& s)
            {
                s.as_state()._deferred_fns.execute_all(proxy);
            });
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TF>
    decltype(auto) instance<TSettings, TSystemSignature>::for_outputs(TF && f)
    {
        return this->for_states([this, &f](auto& s)
            {
                f(this->system(), s.as_data());
            });
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
        return subscribed().add(eid);
    }

    template <typename TSettings, typename TSystemSignature>
    auto instance<TSettings, TSystemSignature>::unsubscribe(entity_id eid)
    {
        return subscribed().erase(eid);
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::prepare_and_wait_subtasks(
        TContext & ctx, sz_t n, TF & f)
    {
        // Assert that at least one subtask is being executed.
        ECST_ASSERT_OP(n, >, 0);

        // Prepare `n` states, but set the counter to `n - 1` since one of the
        // subtasks will be executed in the current thread.
        this->state_manager().clear_and_prepare(n);
        counter_blocker b{n - 1};

        // Function accepting a callable object which will be executed in a
        // separate thread. Intended to be called from inner parallelism
        // strategy executors.
        auto run_in_separate_thread = [this, &ctx, &b](auto& xf)
        {
            return [this, &b, &ctx, &xf](auto&&... xs) mutable
            {
                // Use of multithreading:
                // * Run subtask slices in separate threads.
                ctx.post_in_thread_pool([&xf, &b, xs...]() mutable
                    {
                        xf(FWD(xs)...);
                        b.decrement_and_notify_all();
                    });
            };
        };

        // Runs the parallel executor and waits until the remaining subtasks
        // counter is zero.
        b.execute_and_wait_until_zero([&f, &run_in_separate_thread]
            {
                f(run_in_separate_thread);
            });
    }

    template <typename TSettings, typename TSystemSignature>
    void instance<TSettings, TSystemSignature>::prepare_single_subtask()
    {
        this->state_manager().clear_and_prepare(1);
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::execute_single( // .
        TContext & ctx, TF & f                                  // .
        )
    {
        prepare_single_subtask();

        // Create single-subtask data proxy.
        auto dp = data_proxy::make_single<TSystemSignature>(*this, ctx);
        f(dp);
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::execute_in_parallel( // .
        TContext & ctx, TF & f                                       // .
        )
    {
        auto st = [ this, &ctx, f = FWD(f) ] // .
            (auto split_idx, auto i_begin, auto i_end) mutable
        {
            // Create multi data proxy.
            auto dp = data_proxy::make_multi<TSystemSignature>(
                *this, ctx, split_idx, i_begin, i_end);

            // Execute the bound slice.
            f(dp);
        };

        this->parallel_executor().execute(*this, ctx, std::move(st));
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext>
    auto instance<TSettings, TSystemSignature>::execution_dispatch(
        TContext & ctx) noexcept
    {
        return [this, &ctx](auto&& f)
        {
            return static_if(settings::inner_parallelism_allowed(TSettings{}))
                .then([this, &ctx](auto&& xf)
                    {
                        return this->execute_in_parallel(ctx, FWD(xf));
                    })
                .else_([this, &ctx](auto&& xf)
                    {
                        return this->execute_single(ctx, FWD(xf));
                    })(FWD(f));
        };
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    void instance<TSettings, TSystemSignature>::execute( // .
        TContext & ctx, TF & f                           // .
        )
    {
        // Bind the dispatch function to `ctx`.
        auto bound_dispatch = execution_dispatch(ctx);

        // Create an executor proxy.
        auto ep = executor_proxy::make(*this, std::move(bound_dispatch));

        // Call the user `(instance&, executor&)` function.
        f(*this, ep);
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::subscribed_count()
        const noexcept
    {
        return this->subscribed().size();
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
