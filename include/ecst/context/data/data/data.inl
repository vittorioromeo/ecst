// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../defer.hpp"
#include "./data.hpp"
#include <ecst/utils.hpp>

namespace ecst::context
{
    namespace impl
    {
        using handle = ecst::context::entity::impl::handle;

        template <typename TSettings>
        template <typename TFStep, typename... TFsRefresh>
        auto data<TSettings>::step(TFStep&& f_step, TFsRefresh&&... fs_refresh)
        {
            // Create an overloaded `refresh_event` handler with a `do_nothing`
            // fallback.
            auto refresh_event_handler = bh::overload_linearly(
                FWD(fs_refresh)..., ecst::impl::do_nothing);

            // Ensure `refresh()` is called after executing `f`.
            ECST_SCOPE_GUARD([this, reh = std::move(refresh_event_handler)] {
                this->refresh(std::move(reh));
            });

            // Clear refresh state.
            _refresh_state.clear();

            // Build context step proxy.
            step_proxy_type step_proxy{*this, _refresh_state};

            // Execute user-defined step.
            return f_step(step_proxy);
        }

        template <typename TSettings>
        template <typename TFRefresh>
        void data<TSettings>::refresh(TFRefresh&& f_refresh)
        {
            // Execute deferred functions, filling up the refresh state and
            // allocating memory if necessary.
            refresh_impl_execute_deferred(_refresh_state, f_refresh);

            // Unsubscribe all killed entities from systems.
            refresh_impl_kill_entities(_refresh_state, f_refresh);

            // Match all modified and new entities to systems.
            refresh_impl_match_entities(_refresh_state, f_refresh);
        }

        template <typename TSettings>
        template <typename TRefreshState, typename TFRefresh>
        void data<TSettings>::refresh_impl_execute_deferred(
            TRefreshState& rs, TFRefresh&&)
        {
            ELOG(                                        // .
                debug::lo_context_refresh()              // .
                    << "Executing deferred functions\n"; // .
            );

            defer_proxy_type defer_proxy{*this, rs};

            // Sequentially execute every deferred function.
            this->for_stateful_instances_sequential(
                [&defer_proxy](auto& instance) {
                    instance.for_states([&defer_proxy](auto& s) {
                        // The execution of deferred functions fills the
                        // refresh state and alters the context state.
                        s.as_state()._deferred_fns.execute_all(defer_proxy);
                    });
                });
        }

        template <typename TSettings>
        template <typename TRefreshState, typename TFRefresh>
        void data<TSettings>::refresh_impl_kill_entities(
            TRefreshState& rs, TFRefresh&& f_refresh)
        {
            ELOG(                                        // .
                debug::lo_context_refresh()              // .
                    << "Killing marked dead entities\n"; // .
            );

            // Sequentially add entities to kill in the main refresh state
            // sparse set, and clear instance subtask states.
            this->for_stateful_instances_sequential([&rs](auto& instance) {
                instance.for_states([&rs](auto& s) {
                    s.as_state()._to_kill.for_each(
                        [&rs](entity_id eid) { rs._to_kill.add(eid); });

                    // Clear deferred functions and to-kill sets.
                    s.clear();
                });
            });

            // (Possibly due to data locality reasons, it is more efficient
            // to iterate over `rs` twice.)

            // Unsubscribe dead entities from instances, in parallel.
            this->for_entity_instances_dispatch([&rs, &f_refresh](
                                                    auto& instance) {
                rs._to_kill.for_each([&instance, &f_refresh](entity_id eid) {
                    if(instance.unsubscribe(eid))
                    {
                        // Fire an event if the unsubscription was
                        // successful.
                        f_refresh(
                            refresh_event::impl::unsubscribed, instance, eid);
                    }
                });
            });

            // Reclaim all killed entities and fire events.
            rs._to_kill.for_each([this, &f_refresh](entity_id eid) {
                this->reclaim(eid);
                f_refresh(refresh_event::impl::reclaimed, eid);
            });
        }

        template <typename TSettings>
        template <typename TRefreshState, typename TFRefresh>
        void data<TSettings>::refresh_impl_match_entities(
            TRefreshState& rs, TFRefresh&& f_refresh)
        {
            ELOG(                                          // .
                debug::lo_context_refresh()                // .
                    << "Matching new/modified entities\n"; // .
            );

            // Match new/modified entities to instances, in parallel.
            this->for_entity_instances_dispatch(
                [this, &rs, &f_refresh](auto& instance) {
                    rs._to_match.for_each(
                        [this, &rs, &instance, &f_refresh](entity_id eid) {
                            // Get entity bitset.
                            const auto& ebs(this->metadata(eid).bitset());

                            ELOG(                               // .
                                auto& sbs(instance.bitset());   // .
                                debug::lo_system_match()        // .
                                << "\n"                         // .
                                << "S:\t" << sbs.str() << "\n"  // .
                                << "E:\t" << ebs.str() << "\n"; // .
                            );

                            // Check if the bitset matches the system.
                            if(instance.matches_bitset(ebs))
                            {
                                ELOG(                              // .
                                    debug::lo_system_match()       // .
                                        << "Matched (eID: " << eid // .
                                        << ") in system ("         // .
                                        << instance.system_id()    // .
                                        << ")\n";                  // .
                                );

                                // If the entity matches the system, subscribe
                                // it.
                                if(instance.subscribe(eid))
                                {
                                    // If the subscription was successful, fire
                                    // an event.
                                    f_refresh(refresh_event::impl::subscribed,
                                        instance, eid);
                                }
                            }
                            else
                            {
                                // If the entity does not match the system,
                                // unsubscribe it.
                                if(instance.unsubscribe(eid))
                                {
                                    // If the unsubscription was successful,
                                    // fire an event.
                                    f_refresh(refresh_event::impl::unsubscribed,
                                        instance, eid);
                                }
                            }
                        });
                });
        }
    } // namespace impl
} // namespace ecst::context
