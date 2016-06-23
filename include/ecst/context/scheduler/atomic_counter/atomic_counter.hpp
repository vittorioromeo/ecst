// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/mp.hpp>
#include <ecst/tag.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/settings.hpp>
#include "./task_group.hpp"
#include "./utils.hpp"

ECST_SCHEDULER_NAMESPACE
{
    namespace impl
    {
        /// @brief Namespace alias for the `atomic_counter` scheduler.
        namespace sac = ecst::scheduler::atomic_counter;
    }

    /// @brief System execution scheduler based on a runtime atomic counter.
    template <typename TSettings>
    class s_atomic_counter
    {
    private:
        static constexpr auto ssl() noexcept
        {
            return settings::system_signature_list(TSettings{});
        }

        impl::sac::impl::task_group_type<decltype(ssl())> _task_group;

        /// @brief Resets all dependency atomic counters.
        void reset() noexcept
        {
            impl::sac::reset_task_group_from_ssl(ssl(), _task_group);
        }

        template <typename TContext, typename TStartSystemTagList,
            typename TBlocker, typename TF>
        void start_execution(
            TContext& ctx, TStartSystemTagList sstl, TBlocker& b, TF&& f)
        {
            namespace sls = signature_list::system;

            // Execution can only be started from independent systems.
            ECST_S_ASSERT(tag::system::is_list(sstl));
            ECST_S_ASSERT_DT(sls::independent_tag_list(ssl(), sstl));

            bh::for_each(sstl, [ this, &ctx, &b, f = FWD(f) ](auto st) mutable
                {
                    auto sid = sls::id_by_tag(this->ssl(), st);

                    ctx.post_in_thread_pool([this, sid, &ctx, &b, &f]() mutable
                        {
                            this->_task_group.start_from_task_id(
                                b, sid, ctx, f);
                        });
                });
        }

    public:
        template <typename TContext, typename TStartSystemTagList, typename TF>
        void execute(TContext& ctx, TStartSystemTagList sstl, TF&& f)
        {
            ECST_S_ASSERT(tag::system::is_list(sstl));

            reset();

            // Number of unique nodes traversed starting from every node in
            // `sstl`.
            constexpr auto chain_size(
                signature_list::system::chain_size(ssl(), sstl));

            // Aggregates the required synchronization objects.
            counter_blocker b(chain_size);

            // Starts every independent task and waits until the remaining tasks
            // counter reaches zero. We forward `f` into the lambda here, then
            // refer to it everywhere else.
            execute_and_wait_until_counter_zero(b,
                [ this, &ctx, sstl, f = FWD(f) ](auto& x_b) mutable
                {
                    this->start_execution(ctx, sstl, x_b, f);
                });
        }
    };
}
ECST_SCHEDULER_NAMESPACE_END

// TODO: possible issue?
/*
    A
     \
      >-C
     /
    B
*/
// Starting from either A or B is incorrect.
// The algorithm should prevent that and either:
// 1) force the user to start both from A and B
// 2) force the user to create a shared depedency between A and B
