// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_NAMESPACE
{
    // TODO: move and
    // * detailed_all
    // * detailed_matching(predicate)
    // * detailed_by_tag(tag)

    namespace system_execution_adapter
    {
        namespace all
        {
            template <typename TF>
            auto detailed(TF&& f) noexcept;

            template <typename TF>
            auto for_subtasks(TF&& f) noexcept;
        }
    }
}
ECST_NAMESPACE_END
