// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include "../defer.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class data;

        namespace step
        {
            template <typename TSettings>
            class proxy : public defer::proxy<TSettings>
            {
            private:
                using settings_type = TSettings;
                using base_type = defer::proxy<settings_type>;
                using context_type = data<settings_type>;
                using refresh_state_type = defer::refresh_state<settings_type>;

            public:
                proxy(context_type&, refresh_state_type&) noexcept;

                template <typename... TFs>
                auto execute_systems(TFs&&... fs);

                // TODO: to global
                // * detailed_all
                // * detailed_matching(predicate)
                // * detailed_by_tag(tag)
                template <typename TF>
                auto detailed_all(TF&& f) noexcept;

                template <typename TSystemTag, typename TF>
                auto detailed(TSystemTag st, TF&& f) noexcept;

                template <typename TSystemTag, typename TF>
                auto for_every_subtask(TSystemTag st, TF&& f) noexcept;
            };
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
