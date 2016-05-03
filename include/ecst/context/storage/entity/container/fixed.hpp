// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <array>
#include <vrm/core/experimental/sparse_set.hpp>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>

ECST_CONTEXT_STORAGE_ENTITY_NAMESPACE
{
    namespace container
    {
        template <typename TEntityMetadata, sz_t TCapacity>
        class fixed
        {
        public:
            static constexpr auto capacity = TCapacity;

        private:
            using entity_metadata_type = TEntityMetadata;

            std::array<entity_metadata_type, capacity> _data;
            vrmc::fixed_vector_sparse_set<entity_id, TCapacity> _free_ids;

            auto valid_index(sz_t i) const noexcept
            {
                return i >= 0 && i < _data.size();
            }

            auto entity_id_to_index(entity_id eid) const noexcept
            {
                return vrmc::to_sz_t(eid);
            }

            auto empty_free_ids() const noexcept
            {
                return _free_ids.empty();
            }

            auto pop_free_id() noexcept
            {
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
            fixed()
            {
                add_range_in_set_reverse(
                    _free_ids, entity_id{0}, entity_id{capacity});
            }

            auto create_and_get_eid() noexcept
            {
                return pop_free_id();
            }

            auto alive(entity_id eid) const noexcept
            {
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
