// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./buffer_base.hpp"
#include <array>
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>

namespace ecst::context::storage::component
{
    namespace chunk
    {
        namespace impl
        {
            struct fixed_buffer_metadata
            {
            };
        } // namespace impl

#define ECST_IMPL_FIXED_BUFFER_BASE_TYPE                   \
    impl::buffer_base<fixed_buffer<TComponentTagList, TN>, \
        impl::fixed_buffer_metadata, TComponentTagList>

        template <typename TComponentTagList, typename TN>
        class fixed_buffer : public ECST_IMPL_FIXED_BUFFER_BASE_TYPE
        {
            friend ECST_IMPL_FIXED_BUFFER_BASE_TYPE;
#undef ECST_IMPL_FIXED_BUFFER_BASE_TYPE

        public:
            using component_tag_list_type = TComponentTagList;
            using metadata = impl::fixed_buffer_metadata;

        private:
            using component_tuple_type =
                impl::component_tuple_type<TComponentTagList>;

            std::array<component_tuple_type, TN{}> _data;

            void grow_if_required(sz_t) const noexcept
            {
            }
        };
    } // namespace chunk
} // namespace ecst::context::storage::component
