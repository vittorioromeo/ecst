// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/mp/list.hpp>

ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE
{
    /// @brief Dependency data of a task.
    /// @tparam TDependentIDList Compile-time list of dependent task IDs.
    template <typename TDependentIDList>
    class task_dependency_data
    {
    private:
        /// @brief Keeps track of the count of the remaining depedencies that
        /// still have to be executed during runtime scheduling.
        std::atomic<sz_t> _remaining_dependencies;

    public:
        /// @brief Decrements the number of remaining dependencies and returns
        /// `true` if there are no more dependencies left.
        auto decrement_and_check() noexcept
        {
            ECST_ASSERT_OP(_remaining_dependencies, >, 0);
            return --_remaining_dependencies == 0;
        }

        /// @brief Initializes the number of remanining dependencies.
        void set_remaining_dependencies(sz_t x) noexcept
        {
            _remaining_dependencies = x;
        }

        /// @brief Executes `f` for every dependent ID.
        template <typename TF>
        void for_dependent_ids(TF&& f)
        {
            bh::for_each(TDependentIDList{}, f);
        }

        auto dependent_tasks_count() const noexcept
        {
            return bh::size(TDependentIDList{});
        }
    };
}
ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE_END
