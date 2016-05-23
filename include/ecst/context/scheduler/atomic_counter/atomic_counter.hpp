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
        static constexpr auto ssl()
        {
            return settings::system_signature_list(TSettings{});
        }

        impl::sac::impl::task_group_type<decltype(ssl())> _task_group;

        /// @brief Resets all dependency atomic counters.
        void reset() noexcept
        {
            impl::sac::reset_task_group_from_ssl(ssl(), _task_group);
        }

        template <typename TContext, typename TSystemTag, typename TBlocker,
            typename TF>
        void start_execution(TContext& ctx, TSystemTag st, TBlocker& b, TF&& f)
        {
            // Execution can only be started from independent systems.
            auto ss = signature_list::system::signature_by_tag(ssl(), st);
            ECST_S_ASSERT_DT(signature::system::is_independent(ss));

            auto sid = signature_list::system::id_by_tag(ssl(), st);

            ctx.post_in_thread_pool([this, sid, &ctx, &b, &f]
                {
                    this->_task_group.start_from_task_id(b, sid, ctx, f);
                });
        }

    public:
        s_atomic_counter() = default;
        ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(s_atomic_counter);

        template <typename TSystemStorage>
        void initialize(TSystemStorage&)
        {
            reset();
        }

        template <typename TContext, typename TSystemTag, typename TF>
        void execute(TContext& ctx, TSystemTag st, TF&& f)
        {
            reset();

            // TODO: count systems starting from st
            // Aggregates the required synchronization objects.
            constexpr auto chain_size(
                signature_list::system::chain_size(ssl(), st));

            std::cout << "chainsz: " << chain_size << std::endl;
            counter_blocker b(chain_size);

            // Starts every independent task and waits until the remaining tasks
            // counter reaches zero. We forward `f` into the lambda here, then
            // refer to it everywhere else.
            execute_and_wait_until_counter_zero(b,
                [ this, &ctx, &b, st, f = FWD(f) ]() mutable
                {
                    this->start_execution(ctx, st, b, f);
                });
        }
    };
}
ECST_SCHEDULER_NAMESPACE_END
