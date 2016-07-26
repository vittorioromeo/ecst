// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils/sparse_int_set.hpp>
#include <ecst/mp.hpp>
#include <ecst/signature.hpp>
#include <ecst/signature_list.hpp>
#include <ecst/inner_parallelism.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/types.hpp>
#include <ecst/context/bitset.hpp>
#include "./base.hpp"
#include "../state.hpp"

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    /// @brief System instance.
    /// @details Contains:
    /// * An instance of the user-defined system type.
    /// * A state manager.
    /// * A sparse set of subscribed entities.
    /// * A bitset of the used components by the system.
    /// * A parallel executor.
    template <typename TSettings, typename TSystemSignature>
    class instance : public impl::instance_base<TSettings, TSystemSignature>
    {
        template <typename, typename, typename, typename>
        friend class data_proxy::base;

        template <typename, typename, typename>
        friend class data_proxy::single;

        template <typename, typename, typename>
        friend class data_proxy::multi;

    private:
        using base_type = impl::instance_base<TSettings, TSystemSignature>;

    public:
        using system_tag_type = typename base_type::system_tag_type;
        using system_type = typename base_type::system_type;

    private:
        using this_type = instance<TSettings, TSystemSignature>;
        using bitset_type = bitset::dispatch<TSettings>;

        using state_manager_type = // .
            impl::state_manager::data<this_type, TSettings, TSystemSignature>;

        using set_type = dispatch_set<TSettings>;

        using parallel_parameters_type = // .
            signature::system::parallelism_type<TSystemSignature>;

        using parallel_executor_type = // .
            inner_parallelism::executor_type<parallel_parameters_type>;


        state_manager_type _sm;
        set_type _subscribed;
        bitset_type _bitset;
        parallel_executor_type _parallel_executor;

    public:
        auto& subscribed() noexcept;
        const auto& subscribed() const noexcept;
        auto subscribed_count() const noexcept;

    public:
        static constexpr auto system_id() noexcept
        {
            return signature_list::system::id_by_type<system_type>(
                settings::system_signature_list(TSettings{}));
        }

        instance();

        instance(const instance&) = delete;
        instance& operator=(const instance&) = delete;

        instance(instance&&) = default;
        instance& operator=(instance&&) = default;

        /// @brief Executes `f` on all subtask states.
        template <typename TF>
        decltype(auto) for_states(TF&& f);

        /// @brief Executes all subtask states' deferred functions.
        template <typename TProxy>
        void execute_deferred_fns(TProxy& proxy);

        /// @brief Executes `f` on all subtask outputs.
        template <typename TF>
        decltype(auto) for_outputs(TF&& f);

        /// @brief Returns `true` if `eid` is subscribed to the system.
        auto is_subscribed(entity_id eid) const noexcept;

        /// @brief Subscribes `eid` to the system.
        auto subscribe(entity_id eid);

        /// @brief Unsubscribes `eid` from the system.
        auto unsubscribe(entity_id eid);

        const auto& bitset() const noexcept;

        template <typename TBitset>
        auto matches_bitset(const TBitset& b) const noexcept;

    public:
        /// @brief Clears and allocates subtask states.
        /// @details Executes a subtask in the current thread and forwards `n -
        /// 1` subtasks to the thread pool. The `n` value must be at least `1`.
        template <typename TContext, typename TF>
        void prepare_and_wait_subtasks(TContext& ctx, sz_t n, TF& f);

        template <typename TContext, typename TF>
        void execute(TContext& ctx, TF& f);

    private:
        /// @brief Executes `f` on every subscribed entity, without spawning any
        /// additional task.
        template <typename TContext, typename TF>
        void execute_single(TContext& ctx, TF& f);

        /// @brief Executes `f` using the strategy provided by
        /// `_parallel_executor`.
        template <typename TContext, typename TF>
        void execute_in_parallel(TContext& ctx, TF& f);

        /// @brief Returns an execution function that, when called with a
        /// user-defined processing function, either invokes a single-threaded
        /// execution or a multi-threaded execution.
        template <typename TContext>
        auto execution_dispatch(TContext&) noexcept;

        /// @brief Returns the `n`-th subscribed id.
        auto nth_subscribed(sz_t n) noexcept;

        /// @brief Returns a reference to the `n`-th subtask state.
        auto& nth_state(sz_t n) noexcept;

        /// @brief Number of entities used in `for_all_entities`.
        auto all_entity_count() const noexcept;

        /// @brief Number of entities used in `for_entities`.
        auto entity_range_count(sz_t i_begin, sz_t i_end) const noexcept;

        /// @brief Number of entities used in `for_other_entities`.
        auto other_entity_range_count(sz_t i_begin, sz_t i_end) const noexcept;

        /// @brief Execute a function on all entities.
        template <typename TF>
        void for_all_entities(TF&& f);

        /// @brief Execute a function on a range subset of entities.
        template <typename TF>
        void for_entities(sz_t i_begin, sz_t i_end, TF&& f);

        /// @brief Execute a function on all entities except a range subset of
        /// entities.
        template <typename TF>
        void for_other_entities(sz_t i_begin, sz_t i_end, TF&& f);
    };
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END
