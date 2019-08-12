// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/utils.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        namespace impl
        {
            // TODO: use bh tuple ?
            template <typename TComponentTagList>
            using component_tuple_type =
                mp::list::unwrap_tuple<ECST_DECAY_DECLTYPE(
                    TComponentTagList{})>;
        } // namespace impl
    }     // namespace chunk
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
