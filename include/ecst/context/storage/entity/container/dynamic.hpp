// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vector>
#include <vrm/core/experimental/sparse_set.hpp>
#include <vrm/core/experimental/resizable_buffer.hpp>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/utils.hpp>
#include <ecst/debug.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_STORAGE_ENTITY_NAMESPACE
{
    namespace container
    {
        template <typename TSettings, typename TEntityMetadata>
        class dynamic
        {
        public:
            using settings_type = TSettings;

        private:
            using entity_metadata_type = TEntityMetadata;

            vrmc::resizable_buffer<entity_metadata_type> _data;
            vrmc::dynamic_vector_sparse_set<entity_id> _free_ids;
            sz_t _capacity = 0;

            auto valid_index(sz_t i) const noexcept
            {
                return i >= 0 && i < _capacity;
            }

            auto entity_id_to_index(entity_id eid) const noexcept
            {
                return vrmc::to_sz_t(eid);
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
                if(unlikely(empty_free_ids()))
                {
                    grow();
                }
            }

            auto empty_free_ids() const noexcept
            {
                return _free_ids.empty();
            }

            auto pop_free_id()
            {
                grow_if_required();
                ECST_ASSERT(!empty_free_ids());

                auto result = _free_ids.back();
                _free_ids.pop_back();

                return result;
            }

            auto push_free_id(entity_id id) noexcept
            {
                return _free_ids.add(id);
            }

            template <typename TSelf>
            static decltype(auto) get_impl(TSelf&& self, entity_id eid) noexcept
            {
                auto i = self.entity_id_to_index(eid);
                ECST_ASSERT(self.valid_index(i));

                return vrmc::forward_like<TSelf>(self._data[i]);
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

            auto create_and_get_eid()
            {
                return pop_free_id();
            }

            auto alive(entity_id eid) const noexcept
            {
                // If `eid` is in use, then the entity is alive.
                return !_free_ids.has(eid);
            }

            auto reclaim(entity_id eid)
            {
                return push_free_id(eid);
            }

            template <typename... Ts>
                auto& get(Ts&&... xs) & noexcept
            {
                return get_impl(*this, FWD(xs)...);
            }

            template <typename... Ts>
            const auto& get(Ts&&... xs) const& noexcept
            {
                return get_impl(*this, FWD(xs)...);
            }
        };
    }
}
ECST_CONTEXT_STORAGE_ENTITY_NAMESPACE_END
