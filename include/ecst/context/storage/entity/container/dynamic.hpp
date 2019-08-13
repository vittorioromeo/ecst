// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./base.hpp"
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <vector>
#include <vrm/core/experimental/resizable_buffer.hpp>
#include <vrm/core/experimental/sparse_set.hpp>

namespace ecst::context::storage::entity
{
    namespace container
    {
#define ECST_IMPL_DYNAMIC_BASE_TYPE \
    impl::base<dynamic<TSettings, TEntityMetadata>, TEntityMetadata>

        template <typename TSettings, typename TEntityMetadata>
        class dynamic : public ECST_IMPL_DYNAMIC_BASE_TYPE
        {
            friend ECST_IMPL_DYNAMIC_BASE_TYPE;
#undef ECST_IMPL_DYNAMIC_BASE_TYPE

        public:
            using settings_type = TSettings;

        private:
            using entity_metadata_type = TEntityMetadata;

            vrmc::resizable_buffer<entity_metadata_type> _data;
            vrmc::dynamic_vector_sparse_set<entity_id> _free_ids;
            sz_t _capacity = 0;

            auto capacity() const noexcept
            {
                return _capacity;
            }

            void grow_by(sz_t n)
            {
                // If both `_capacity` and `n` are zero, we get an overflow.
                ECST_ASSERT(_capacity != 0 || n != 0);

                ELOG(                         // .
                    debug::lo_entity_memory() // .
                        << "Growing dynamic entity container (from "
                        << _capacity                         // .
                        << " to " << _capacity + n << ")\n"; // .
                );

                add_range_in_set_reverse(
                    _free_ids, entity_id{_capacity}, entity_id{_capacity + n});

                auto new_capacity = _capacity + n;
                _data.grow_and_construct(_capacity, new_capacity);
                _capacity = new_capacity;

                ELOG(                         // .
                    debug::lo_entity_memory() // .
                        << "New occupied memory by dynamic entity container: " // .
                        << (new_capacity * sizeof(entity_metadata_type)) /
                               1024.f / 1024.f // .
                        << "MB\n";             // .
                );
            }

            void grow()
            {
                grow_by((_capacity + 10) * 0.75f);
            }

            void grow_if_required()
            {
                if(unlikely(this->empty_free_ids()))
                {
                    grow();
                }
            }

        public:
            dynamic()
            {
                grow_by(settings_type{}.get_dynamic_capacity() + 10);
            }

            ~dynamic()
            {
                _data.destroy_and_deallocate(_capacity);
            }

            ECST_DEFINE_DEFAULT_MOVE_ONLY_OPERATIONS(dynamic);
        };
    } // namespace container
} // namespace ecst::context::storage::entity
