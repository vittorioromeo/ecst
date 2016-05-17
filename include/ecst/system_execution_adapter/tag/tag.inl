// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/signature.hpp>
#include "./tag.hpp"

ECST_NAMESPACE
{
    namespace system_execution_adapter
    {
        namespace tag
        {
            template <typename TSystemTag, typename TF>
            auto detailed(TSystemTag, TF&& f) noexcept
            {
                using system_type = signature::system::unwrap_tag<TSystemTag>;

                return [f = FWD(f)](system_type & s, auto& executor)
                {
                    f(s, executor);
                };
            }

            template <typename TSystemTag, typename TF>
            auto for_subtasks(TSystemTag st, TF&& f) noexcept
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
ECST_NAMESPACE_END
