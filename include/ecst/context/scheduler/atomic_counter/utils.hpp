// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/signature.hpp>
#include <ecst/signature_list.hpp>
#include "./task_dependency_data.hpp"
#include "./task.hpp"
#include "./task_group.hpp"

ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE
{
    /// @brief Resets a task group's dependency data.
    /// @details Given a system signature list, sets all the dependency counters
    /// to the number of original dependencies.
    template <typename TSSL, typename TTaskGroup>
    auto reset_task_group_from_ssl(TSSL ssl, TTaskGroup & tg) noexcept
    {
        namespace ss = signature::system;
        namespace sls = signature_list::system;

        // For every ID inside `ssl`...
        sls::for_ids(
            [&tg, &ssl](auto s_id)
            {
                // Retrieve the system signature.
                auto s_sig = sls::signature_by_id(ssl, s_id);

                // Retrieve the corresponding task.
                auto& t = tg.task_by_id(s_id);

                // Set the number of dependencies to the correct one.
                t.dependency_data().set_remaining_dependencies( // .
                    ss::dependency_count(s_sig));
            },
            ssl);
    }

    namespace impl
    {
        template <typename TSSL>
        auto task_group_transformer(TSSL ssl) noexcept
        {
            namespace ss = signature::system;
            namespace sls = signature_list::system;

            return mp::bh::transform(ssl, [=](auto ss)
                {
                    // Get the list of dependent IDs of `ss`.
                    auto dependent_ids = sls::dependent_ids_list(ssl, ss);
                    using dep_list_type = decltype(dependent_ids);

                    // Use the list to compute the task type.
                    using dep_data_type = task_dependency_data<dep_list_type>;
                    using task_type = task<dep_data_type>;

                    // Wrap the task type.
                    return mp::type_c<task_type>;
                });
        }

        /// @brief Type of task group for a specific system signature list.
        /// @details All `type_c` wrappers are unwrapped thanks to
        /// `unwrap_tuple`.
        template <typename TSSL>
        using task_group_type = task_group<              // .
            mp::list::unwrap_tuple<                      // .
                decltype(task_group_transformer(TSSL{})) // .
                >                                        // .
            >;
    }
}
ECST_SCHEDULER_ATOMIC_COUNTER_NAMESPACE_END
