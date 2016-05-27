// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace defer
        {
            template <typename TSettings>
            class refresh_state
                : public ecst::impl::to_kill_set_wrapper<TSettings>
            {
            private:
                using settings_type = TSettings;
                using set_type = dispatch_set<settings_type>;

                set_type _to_match_ids;

            public:
                void clear() noexcept;

                void add_to_match(entity_id eid) // .
                    noexcept(noexcept(           // .
                        _to_match_ids.add(eid)   // .
                        ));

                template <typename TF>
                void for_to_match(TF&& f)              // .
                    noexcept(noexcept(                 // .
                        _to_match_ids.for_each(FWD(f)) // .
                        ));
            };
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
