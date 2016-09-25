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
#include <ecst/mp.hpp>
#include <ecst/context/bitset.hpp>
#include <ecst/context/scheduler.hpp>
#include <ecst/context/system.hpp>
#include <ecst/context/storage.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        // TODO:
        // Kind aliases.
        constexpr auto k_stateless = signature::system::impl::kind::stateless;
        constexpr auto k_stateful = signature::system::impl::kind::stateful;
        constexpr auto k_entity = signature::system::impl::kind::entity;

        /// @brief Class managing systems and threads.
        /// @details Contains a thread pool, and a system storage.
        template <typename TSettings>
        class system_manager
        {
            template <typename>
            friend class data;

        public:
            using settings_type = TSettings;
            using thread_pool = ecst::thread_pool;

            using system_storage_type = // .
                context::storage::system::dispatch<settings_type>;

            using scheduler_type = // .
                typename settings::impl::ctx_scheduler<
                    settings_type>::template instantiate<settings_type>;

            template <typename T>
            using system_from_tag = tag::system::unwrap<T>;

        private:
            std::unique_ptr<thread_pool> _thread_pool{
                std::make_unique<thread_pool>()};

            system_storage_type _system_storage;
            scheduler_type _scheduler;

            // TODO: to inl
            constexpr static auto provider_all_instances() noexcept
            {
                return [](auto&& ss, auto&&... xs)
                {
                    ss.for_all_instances(FWD(xs)...);
                };
            }

            // TODO: to inl
            template <typename TKind>
            constexpr static auto provider_instances_of_kind(
                TKind kind) noexcept
            {
                return [&kind](auto&& ss, auto&&... xs)
                {
                    ss.for_instances_of_kind(kind, FWD(xs)...);
                };
            }

            // TODO: to inl
            constexpr static auto counter_all_instances() noexcept
            {
                return [](auto&& ss)
                {
                    return ss.all_instances_count();
                };
            }

            // TODO: to inl
            template <typename TKind>
            constexpr static auto counter_instances_of_kind(TKind kind) noexcept
            {
                return [&kind](auto&& ss)
                {
                    return ss.instances_of_kind_count(kind);
                };
            }

            // TODO: to inl
            template <typename TSelf, typename TFInstanceProvider, typename TF>
            static void for_instances_sequential_impl(
                TSelf&& self, TFInstanceProvider&& f_instance_provider, TF&& f)
            {
                f_instance_provider(self._system_storage, FWD(f));
            }

            // TODO: to inl
            template <typename TSelf, typename TFInstanceCounter,
                typename TFInstanceProvider, typename TF>
            static void for_instances_parallel_impl(TSelf&& self,
                TFInstanceCounter&& f_instance_counter,
                TFInstanceProvider&& f_instance_provider, TF&& f)
            {
                // Block until `f` has been called on all instances.
                counter_blocker b{f_instance_counter(self._system_storage)};
                b.execute_and_wait_until_zero(
                    [&self, &f_instance_provider, &b, f = FWD(f) ]
                    {
                        f_instance_provider(self._system_storage,
                            [&self, &b, &f](auto& system)
                            {
                                // Use of multithreading:
                                // * Unsubscribe dead entities from instances.
                                // * Match new/modified entities to instances.
                                self.post_in_thread_pool([&b, &system, &f]
                                    {
                                        f(system);
                                        b.decrement_and_notify_one();
                                    });
                            });
                    });
            }

            template <typename TFPar, typename TFSeq>
            static auto for_instances_dispatch_impl(
                TFPar&& f_par, TFSeq&& f_seq) noexcept
            {
                return [ f_par = FWD(f_par), f_seq = FWD(f_seq) ](auto&& f)
                {
                    static_if(
                        settings::refresh_parallelism_allowed<settings_type>())
                        .then([&f_par](auto&& xf)
                            {
                                f_par(FWD(xf));
                            })
                        .else_([&f_seq](auto&& xf)
                            {
                                f_seq(FWD(xf));
                            })(FWD(f));
                };
            }


        public:
            /// @brief Executes `f` on all systems, sequentially.
            template <typename TF>
            void for_instances_sequential(TF&& f);

            /// @brief Executes `f` on all systems, in parallel.
            template <typename TF>
            void for_instances_parallel(TF&& f);

            /// @brief Executes `f` on all systems, in parallel if enabled by
            /// settings, sequentially otherwise.
            template <typename TF>
            void for_instances_dispatch(TF&& f);

            /// @brief Executes `f` on stateful systems, sequentially.
            template <typename TF>
            void for_stateful_instances_sequential(TF&& f);

            /// @brief Executes `f` on all entity systems, sequentially.
            template <typename TF>
            void for_entity_instances_sequential(TF&& f);

            /// @brief Executes `f` on all entity systems, in parallel.
            template <typename TF>
            void for_entity_instances_parallel(TF&& f);

            /// @brief Executes `f` on all entity systems, in parallel if
            /// enabled by settings, sequentially otherwise.
            template <typename TF>
            void for_entity_instances_dispatch(TF&& f);


            template <typename TF>
            auto post_in_thread_pool(TF&& f);

            template <typename TID>
            auto& instance_by_id(TID) noexcept;

        private:
            /// @brief Executes all the system chains starting with tags in
            /// `sstl`, overloading `fs...`.
            template <typename TContext, typename TStartSystemTagList,
                typename... TFs>
            void execute_systems_impl(
                TContext&, TStartSystemTagList sstl, TFs&&... fs);

        protected:
            /// @brief Returns a variadic lambda accepting system execution
            /// functions that will be executed on all dependency chains
            /// starting from `sts...`.
            template <typename TContext, typename... TStartSystemTags>
            auto execute_systems_from(
                TContext& context, TStartSystemTags... sts) noexcept;

            /// @brief Returns a variadic lambda accepting system execution
            /// functions that will be executed on all dependency chains
            /// starting from all independent systems in the context.
            template <typename TContext>
            auto execute_systems(TContext& context) noexcept;

        public:
            template <typename TSystemTag, typename TF>
            decltype(auto) for_system_outputs(TSystemTag, TF&& f);

            template <typename TSystemTag>
            auto& instance(TSystemTag) noexcept;

            template <typename TSystemTag>
            const auto& instance(TSystemTag) const noexcept;

            template <typename TSystemTag>
            auto& system(TSystemTag) noexcept;

            template <typename TSystemTag>
            const auto& system(TSystemTag) const noexcept;

            template <typename TSystemTag>
            auto is_in_system(TSystemTag, entity_id) const noexcept;

            template <typename TSystemTag>
            auto count_entities_in(TSystemTag) const noexcept;

            template <typename TSystemTag>
            auto any_entity_in(TSystemTag) const noexcept;

            // TODO:
            constexpr auto inner_parallelism_allowed() const noexcept;
        };
    }
}
ECST_CONTEXT_NAMESPACE_END
