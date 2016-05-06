// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./step.hpp"
#include "../data/data.hpp"

ECST_CONTEXT_NAMESPACE
{
    namespace impl
    {
        namespace step
        {
            template <typename TSettings>
            proxy<TSettings>::proxy(context_type& context,
                refresh_state_type& refresh_state) noexcept
                : base_type{context, refresh_state}
            {
            }

            template <typename TSettings>
            template <typename... TFs>
            auto proxy<TSettings>::execute_systems(TFs&&... fs)
            {
                return this->context().execute_systems(
                    this->context(), FWD(fs)...);
            }

            // TODO: maybe free functions?
            template <typename TSettings>
            template <typename TSystemTag, typename TF>
            auto proxy<TSettings>::detailed(TSystemTag, TF&& f) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;

                return [f = FWD(f)](system_type & s, auto& executor)
                {
                    f(s, executor);
                };
            }

            template <typename TSettings>
            template <typename TSystemTag, typename TF>
            auto proxy<TSettings>::for_every_subtask(
                TSystemTag st, TF&& f) noexcept
            {
                return detailed(st, [f = FWD(f)](auto& s, auto& executor)
                    {
                        executor.for_subtasks([&s, &f](auto& data)
                            {
                                f(s, data);
                            });
                    });
            }
        }
    }
}
ECST_CONTEXT_NAMESPACE_END
