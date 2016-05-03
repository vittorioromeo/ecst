// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "../defer.hpp"
#include "./data.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        using handle = ecst::context::entity::impl::handle;

        template <typename TSettings>
        template <typename TRefreshState>
        void data<TSettings>::refresh_impl_execute_deferred(TRefreshState& rs)
        {
            ELOG(                                        // .
                debug::lo_context_refresh()              // .
                    << "Executing deferred functions\n"; // .
                );

            defer_proxy_type defer_proxy{*this, rs};

            this->for_systems_sequential([&defer_proxy](auto& system)
                {
                    system.for_states([&defer_proxy](auto& state)
                        {
                            // The execution of deferred functions fills the
                            // refresh state and alters the context state.
                            state._state.execute_deferred_fns(defer_proxy);
                        });
                });
        }

        template <typename TSettings>
        template <typename TRefreshState>
        void data<TSettings>::refresh_impl_kill_entities(TRefreshState& rs)
        {
            ELOG(                                        // .
                debug::lo_context_refresh()              // .
                    << "Killing marked dead entities\n"; // .
                );

            {
                this->for_systems_sequential([&rs](auto& system)
                    {
                        system.for_states([&rs](auto& state)
                            {
                                state._state.for_to_kill([&rs](auto eid)
                                    {
                                        rs.add_to_kill(entity_id{eid});
                                    });
                            });
                    });
            }

            {
                // Possibly due to data locality reasons, it is more efficient
                // to iterate over `rs` twice.

                this->for_systems_dispatch([this, &rs](auto& system)
                    {
                        rs.for_to_kill([&system](auto eid_to_kill)
                            {
                                system.unsubscribe(entity_id{eid_to_kill});
                            });
                    });

                rs.for_to_kill([this](auto eid_to_kill)
                    {
                        this->reclaim(entity_id{eid_to_kill});
                    });
            }
        }

        template <typename TSettings>
        template <typename TRefreshState>
        void data<TSettings>::refresh_impl_match_entities(TRefreshState& rs)
        {
            ELOG(                                          // .
                debug::lo_context_refresh()                // .
                    << "Matching new/modified entities\n"; // .
                );

            this->for_systems_dispatch([this, &rs](auto& system)
                {
                    rs.for_to_match([this, &system](auto eid)
                        {
                            entity_id eeid(eid);

                            // Get entity metadata.
                            auto& em(this->metadata(eeid));

                            // Get entity bitset.
                            auto& ebs(em.bitset());

                            ELOG(                               // .
                                auto& sbs(system.bitset());     // .
                                debug::lo_system_match()        // .
                                << "\n"                         // .
                                << "S:\t" << sbs.str() << "\n"  // .
                                << "E:\t" << ebs.str() << "\n"; // .
                                );

                            // Check if the bitset matches the system.
                            if(system.matches_bitset(ebs))
                            {
                                ELOG(                              // .
                                    debug::lo_system_match()       // .
                                        << "Matched (eID: " << eid // .
                                        << ") in system ("         // .
                                        << system.system_id()      // .
                                        << ")\n";                  // .
                                    );

                                system.subscribe(eeid);
                            }
                            else
                            {
                                system.unsubscribe(eeid);
                            }
                        });
                });
        }

        template <typename TSettings>
        void data<TSettings>::refresh_impl()
        {
            // Execute deferred functions, filling up the refresh state and
            // allocating memory if necessary.
            refresh_impl_execute_deferred(_refresh_state);

            // Unsubscribe all killed entities from systems.
            refresh_impl_kill_entities(_refresh_state);

            // Match all modified and new entities to systems.
            refresh_impl_match_entities(_refresh_state);
        }

        template <typename TSettings>
        void data<TSettings>::refresh()
        {
            refresh_impl();
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
