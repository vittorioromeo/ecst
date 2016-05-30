// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./refresh_state.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace defer
        {
            template <typename TSettings>
            void refresh_state<TSettings>::clear() noexcept
            {
                ecst::impl::to_kill_set_wrapper<TSettings>::clear();
                _to_match_ids.clear();
            }

            template <typename TSettings>
            void refresh_state<TSettings>::add_to_match(entity_id eid) // .
                noexcept(noexcept(                                     // .
                    _to_match_ids.add(eid)                             // .
                    ))
            {
                _to_match_ids.add(eid);
            }

            template <typename TSettings>
            void refresh_state<TSettings>::del_to_match(entity_id eid) // .
                noexcept(noexcept(                                     // .
                    _to_match_ids.erase(eid)                           // .
                    ))
            {
                _to_match_ids.erase(eid);
            }

            template <typename TSettings>
            template <typename TF>
            void refresh_state<TSettings>::for_to_match(TF&& f) // .
                noexcept(noexcept(                              // .
                    _to_match_ids.for_each(FWD(f))              // .
                    ))
            {
                _to_match_ids.for_each(FWD(f));
            }
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
