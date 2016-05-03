// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vector>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils/sparse_int_set.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/inner_parallelism.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/types.hpp>
#include <ecst/context/bitset.hpp>
#include "./instance.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::nth_subscribed(
        sz_t n) noexcept
    {
        return _subscribed.at(n);
    }

    template <typename TSettings, typename TSystemSignature>
    auto& ECST_PURE_FN instance<TSettings, TSystemSignature>::nth_state(
        sz_t n) noexcept
    {
        return _sm.get(n);
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::all_entity_count()
        const noexcept
    {
        return subscribed_count();
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings, TSystemSignature>::entity_range_count(
        sz_t i_begin, sz_t i_end) const noexcept
    {
        return i_end - i_begin;
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN instance<TSettings,
        TSystemSignature>::other_entity_range_count(sz_t i_begin, sz_t i_end)
        const noexcept
    {
        return (i_begin - 0) + (subscribed_count() - i_end);
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN
    instance<TSettings, TSystemSignature>::make_all_entity_provider() noexcept
    {
        return [this](auto&& f)
        {
            return this->_subscribed.for_each(f);
        };
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN
    instance<TSettings, TSystemSignature>::make_entity_range_provider(
        sz_t i_begin, sz_t i_end) noexcept
    {
        // Assert that the range contains entities.
        ECST_ASSERT(i_end - i_begin > 0);

        return [this, i_begin, i_end](auto&& f)
        {
            for(sz_t i = i_begin; i < i_end; ++i)
            {
                f(this->nth_subscribed(i));
            }
        };
    }

    template <typename TSettings, typename TSystemSignature>
    auto ECST_PURE_FN
    instance<TSettings, TSystemSignature>::make_other_entity_range_provider(
        sz_t i_begin, sz_t i_end) noexcept
    {
        // Assert that the range contains entities.
        ECST_ASSERT(i_end - i_begin > 0);

        return [this, i_begin, i_end](auto&& f)
        {
            for(sz_t i = 0; i < i_begin; ++i)
            {
                f(this->nth_subscribed(i));
            }

            for(sz_t i = i_end; i < this->subscribed_count(); ++i)
            {
                f(this->nth_subscribed(i));
            }
        };
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext>
    auto ECST_PURE_FN
    instance<TSettings, TSystemSignature>::make_entity_range_data(
        TContext & ctx, sz_t state_idx, sz_t i_begin, sz_t i_end) noexcept
    {
        // Entity IDs provider functions.
        auto erp = make_entity_range_provider(i_begin, i_end);
        auto aep = make_all_entity_provider();
        auto oep = make_other_entity_range_provider(i_begin, i_end);

        // Entity counts.
        auto erc = entity_range_count(i_begin, i_end);
        auto aec = all_entity_count();
        auto oec = other_entity_range_count(i_begin, i_end);

        // Execution data instance.
        // The bound state instance is retrieved through`state_idx`.
        return make_data(   // .
            std::move(erp), // .
            erc,            // .
                            // .
            std::move(aep), // .
            aec,            // .
                            // .
            std::move(oep), // .
            oec,            // .
                            // .
            ctx,            // .
            [ this, state_idx ]() -> auto&
            {
                return this->nth_state(state_idx);
            });
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TCounterBlocker, typename TContext>
    auto ECST_PURE_FN instance<TSettings,
        TSystemSignature>::make_slice_executor(TCounterBlocker & cb,
        TContext & ctx, sz_t state_idx, sz_t i_begin, sz_t i_end) noexcept
    {
        return [this, &cb, &ctx, state_idx, i_begin, i_end](auto&& f)
        {
            auto data =
                this->make_entity_range_data(ctx, state_idx, i_begin, i_end);

            // Executes the processing function over the slice of entities.
            this->execute_subtask_and_decrement_counter(cb, data, f);
        };
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TCounterBlocker, typename TContext, typename TF>
    auto instance<TSettings, TSystemSignature>::make_bound_slice_executor(
        TCounterBlocker & cb, TContext & ctx, sz_t state_idx, sz_t i_begin,
        sz_t i_end, TF && f) noexcept
    {
        return [this, &cb, &ctx, &f, state_idx, i_begin, i_end]
        {
            this->make_slice_executor(cb, ctx, state_idx, i_begin, i_end)(f);
        };
    }

    template <typename TSettings, typename TSystemSignature>
    template <typename TContext, typename TF>
    auto instance<TSettings, TSystemSignature>::run_subtask_in_thread_pool(
        TContext & ctx, TF && f)
    {
        ctx.post_in_thread_pool(FWD(f));
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
