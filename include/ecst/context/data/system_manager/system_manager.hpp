// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/thread_pool.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/bitset.hpp>
#include <ecst/context/scheduler.hpp>
#include <ecst/context/system.hpp>
#include <ecst/context/storage.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        /// @brief Class managing systems and threads.
        /// @details Contains a thread pool, a system runner and a system
        /// storage.
        template <typename TSettings>
        class system_manager
        {
            template <typename>
            friend class data;

        public:
            using settings_type = TSettings;
            using thread_pool = ecst::thread_pool;

            using system_runner_type = // .
                context::system::runner<settings_type>;

            using system_storage_type = // .
                context::storage::system::dispatch<settings_type>;

            template <typename T>
            using system_from_tag = signature::system::unwrap_tag<T>;

        private:
            thread_pool _thread_pool;
            system_runner_type _system_runner;
            system_storage_type _system_storage;

        public:
            system_manager();
            ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(system_manager);

            /// @brief Executes `f` on all systems, sequentially.
            template <typename TF>
            void for_systems_sequential(TF&& f);

            /// @brief Executes `f` on all systems, in parallel.
            template <typename TF>
            void for_systems_parallel(TF&& f);

            /// @brief Executes `f` on all systems, in parallel if enabled by
            /// settings, sequentially otherwise.
            template <typename TF>
            void for_systems_dispatch(TF&& f);

            template <typename TF>
            auto post_in_thread_pool(TF&& f);

            template <typename TID>
            auto& instance_by_id(TID) noexcept;

        protected:
            template <typename TContext, typename... TFs>
            void execute_systems(TContext&, TFs&&... fs);

        public:
            template <typename TSystem, typename TF>
            decltype(auto) for_system_outputs(TF&& f);

            template <typename TSystemTag, typename TF>
            decltype(auto) for_system_outputs(TSystemTag, TF&& f);

            template <typename TSystem, typename TAcc, typename TF>
            auto foldl_system_outputs(TAcc acc, TF&& f);

            template <typename TSystemTag, typename TAcc, typename TF>
            auto foldl_system_outputs(TSystemTag, TAcc acc, TF&& f);

            template <typename TSystem>
            auto& instance() noexcept;

            template <typename TSystem>
            const auto& instance() const noexcept;

            template <typename TSystemTag>
            auto& instance(TSystemTag) noexcept;

            template <typename TSystemTag>
            const auto& instance(TSystemTag) const noexcept;

            template <typename TSystem>
            auto& system() noexcept;

            template <typename TSystem>
            const auto& system() const noexcept;

            template <typename TSystemTag>
            auto& system(TSystemTag) noexcept;

            template <typename TSystemTag>
            const auto& system(TSystemTag) const noexcept;

            template <typename TSystem>
            auto is_in_system(entity_id) const noexcept;

            template <typename TSystemTag>
            auto is_in_system(TSystemTag, entity_id) const noexcept;

            template <typename TSystem>
            auto count_entities_in() const noexcept;

            template <typename TSystemTag>
            auto count_entities_in(TSystemTag) const noexcept;

            template <typename TSystem>
            auto any_entity_in() const noexcept;

            template <typename TSystemTag>
            auto any_entity_in(TSystemTag) const noexcept;
        };
    }
}
ECST_CONTEXT_NAMESPACE_END
