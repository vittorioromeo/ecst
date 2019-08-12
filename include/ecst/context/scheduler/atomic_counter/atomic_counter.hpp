// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./task_group.hpp"
#include "./utils.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/mp.hpp>
#include <ecst/settings.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/tag.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/utils.hpp>

ECST_SCHEDULER_NAMESPACE
{
    namespace impl
    {
        /// @brief Namespace alias for the `atomic_counter` scheduler.
        namespace sac = ecst::scheduler::atomic_counter;
    } // namespace impl

    /// @brief System execution scheduler based on a runtime atomic counter.
    template <typename TSettings>
    class s_atomic_counter
    {
    private:
        static constexpr auto ssl() noexcept
        {
            return settings::system_signature_list(TSettings{});
        }

        using my_task_group = // .
            impl::sac::impl::task_group_type<decltype(ssl())>;

        /// @brief Resets all dependency atomic counters.
        void reset(my_task_group& tg) noexcept
        {
            impl::sac::reset_task_group_from_ssl(ssl(), tg);
        }

        template <typename TContext, typename TStartSystemTagList,
            typename TLatch, typename TF>
        void start_execution(my_task_group& tg, TContext& ctx,
            TStartSystemTagList sstl, TLatch& b, TF& f)
        {
            namespace sls = signature_list::system;

            // Execution can only be started from independent systems.
            static_assert(tag::system::is_list(sstl));
            ECST_S_ASSERT_DT(sls::independent_tag_list(ssl(), sstl));

            bh::for_each(sstl, [this, &tg, &ctx, &b, &f](auto st) mutable {
                auto sid = sls::id_by_tag(this->ssl(), st);

                // Use of multithreading:
                // * Execute multiple indepedent systems in parallel.
                ctx.post_in_thread_pool([sid, &tg, &ctx, &b, &f]() mutable {
                    tg.start_from_task_id(b, sid, ctx, f);
                });
            });
        }

    public:
        template <typename TContext, typename TStartSystemTagList, typename TF>
        void execute(TContext& ctx, TStartSystemTagList sstl, TF& f)
        {
            static_assert(tag::system::is_list(sstl));

            // Number of unique nodes traversed starting from every node in
            // `sstl`.
            constexpr auto chain_size(
                signature_list::system::chain_size(ssl(), sstl));

            // Aggregates the required synchronization objects.
            latch b(chain_size);

            // Create task group.
            my_task_group tg;

            // Starts every independent task and waits until the remaining tasks
            // counter reaches zero.
            b.execute_and_wait_until_zero(
                [this, &tg, &ctx, &b, &f, sstl]() mutable {
                    this->reset(tg);
                    this->start_execution(tg, ctx, sstl, b, f);
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

// Same issue, other test case:
/*
    A
     \
      >-B->D
          /
    C----/
*/
