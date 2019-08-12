// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/utils.hpp>

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace defer
        {
            template <typename TSettings>
            class refresh_state
            {
            private:
                using settings_type = TSettings;
                using set_type = dispatch_set<settings_type>;

            public:
                set_type _to_kill, _to_match;

                void clear() noexcept;
            };
        } // namespace defer
    }     // namespace impl
}
ECST_CONTEXT_NAMESPACE_END
