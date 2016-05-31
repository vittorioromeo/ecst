// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/utils.hpp>
#include "../defer.hpp"
#include "./data.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        using handle = ecst::context::entity::impl::handle;

        template <typename TSettings>
        template <typename TFStep, typename... TFsRefresh>
        auto data<TSettings>::step(TFStep&& f_step, TFsRefresh&&... fs_refresh)
        {
            auto refresh_event_handler = bh::overload_linearly(
                FWD(fs_refresh)..., ecst::impl::do_nothing);

            // Ensure `refresh()` is called after executing `f`.
            ECST_SCOPE_GUARD([ this, reh = std::move(refresh_event_handler) ]
                {
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

            this->for_instances_sequential([&defer_proxy](auto& instance)
                {
                    instance.for_states([&defer_proxy](auto& s)
                        {
                            // The execution of deferred functions fills the
                            // refresh state and alters the context state.
                            s.as_state().execute_deferred_fns(defer_proxy);
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

            this->for_instances_sequential([&rs](auto& instance)
                {
                    instance.for_states([&rs](auto& s)
                        {
                            s.as_state().for_to_kill([&rs](entity_id eid)
                                {
                                    rs.add_to_kill(eid);
                                });

                            // Clear deferred functions and to-kill sets.
                            s.clear();
                        });
                });

            // Possibly due to data locality reasons, it is more efficient
            // to iterate over `rs` twice.

            this->for_instances_dispatch([this, &rs, &f_refresh](auto& instance)
                {
                    rs.for_to_kill([&instance, &f_refresh](entity_id eid)
                        {
                            if(instance.unsubscribe(eid))
                            {
                                f_refresh(refresh_event::impl::unsubscribed,
                                    instance, eid);
                            }
                        });
                });

            rs.for_to_kill([this, &f_refresh, &rs](entity_id eid)
                {
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

            this->for_instances_dispatch([this, &rs, &f_refresh](auto& instance)
                {
                    rs.for_to_match(
                        [this, &rs, &instance, &f_refresh](entity_id eid)
                        {
                            // Get entity metadata.
                            auto& em(this->metadata(eid));

                            // Get entity bitset.
                            auto& ebs(em.bitset());

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

                                if(instance.subscribe(eid))
                                {
                                    f_refresh(refresh_event::impl::subscribed,
                                        instance, eid);
                                }
                            }
                            else
                            {
                                if(instance.unsubscribe(eid))
                                {
                                    f_refresh(refresh_event::impl::unsubscribed,
                                        instance, eid);
                                }
                            }
                        });
                });
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
