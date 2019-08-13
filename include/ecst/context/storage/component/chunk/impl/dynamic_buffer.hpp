// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./buffer_base.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <vector>

namespace ecst::context::storage::component
{
    namespace chunk
    {
        namespace impl
        {
            struct dynamic_buffer_metadata
            {
            };
        } // namespace impl

#define ECST_IMPL_DYNAMIC_BUFFER_BASE_TYPE                          \
    impl::buffer_base<dynamic_buffer<TSettings, TComponentTagList>, \
        impl::dynamic_buffer_metadata, TComponentTagList>

        template <typename TSettings, typename TComponentTagList>
        class dynamic_buffer : public ECST_IMPL_DYNAMIC_BUFFER_BASE_TYPE
        {
            friend ECST_IMPL_DYNAMIC_BUFFER_BASE_TYPE;
#undef ECST_IMPL_DYNAMIC_BUFFER_BASE_TYPE

        public:
            using settings_type = TSettings;
            using component_tag_list_type = TComponentTagList;
            using metadata = impl::dynamic_buffer_metadata;

        private:
            using component_tuple_type =
                impl::component_tuple_type<TComponentTagList>;

            std::vector<component_tuple_type> _data;

            void grow_to(sz_t i)
            {
                auto target(std::max(10 + _data.size() * 2, i + 10));

                ELOG(                            // .
                    debug::lo_component_memory() // .
                        << "Growing dynamic component buffer chunk (i: "
                        << i // .
                        << ") (from " << _data.size() << " to " << target
                        << ")\n"; // .
                );

                _data.resize(target);

                ECST_ASSERT_OP(_data.size(), >, i);
                ECST_ASSERT(this->valid_index(i));

                ELOG(                            // .
                    debug::lo_component_memory() // .
                        << "New occupied memory by dynamic component buffer " // .
                           "chunk: " // .
                        << (_data.capacity() * sizeof(component_tuple_type)) /
                               1024.f / 1024.f // .
                        << "MB\n";             // .
                );
            }

            void grow_if_required(sz_t i)
            {
                if(unlikely(i >= _data.size()))
                {
                    grow_to(i);
                }
            }

        public:
            dynamic_buffer()
            {
                grow_to(settings_type{}.get_dynamic_capacity());
            }
        };
    } // namespace chunk
} // namespace ecst::context::storage::component
