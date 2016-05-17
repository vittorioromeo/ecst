// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/signature.hpp>
#include "./all.hpp"

ECST_NAMESPACE
{
    namespace system_execution_adapter
    {
        namespace all
        {
            template <typename TF>
            auto detailed(TF&& f) noexcept
            {
                return [f = FWD(f)](auto& s, auto& executor)
                {
                    f(s, executor);
                };
            }

            template <typename TF>
            auto for_subtasks(TF&& f) noexcept
            {
                return detailed([f = FWD(f)](auto& s, auto& executor)
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
