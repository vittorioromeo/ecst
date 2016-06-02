// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        namespace impl
        {
            struct empty_metadata
            {
            };
        }

        template <typename TComponentTagList>
        class empty
        {
        public:
            using component_tag_list_type = TComponentTagList;
            using metadata = impl::empty_metadata;

            template <typename... Ts>
            void add(Ts&&...)
            {
            }
        };
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
