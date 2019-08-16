// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/ctx/types.hpp>
#include <ecst/debug.hpp>
#include <ecst/utils.hpp>
#include <vector>
#include <vrm/core/experimental/resizable_buffer.hpp>
#include <vrm/core/experimental/sparse_set.hpp>

namespace ecst::ctx::storage::entity::container::impl
{
    template <typename TDerived, typename TEntityMetadata>
    class base
    {
    private:
        using derived_type = TDerived;
        using entity_metadata_type = TEntityMetadata;

        auto& derived() noexcept
        {
            return vrmc::to_derived<derived_type>(*this);
        }
        const auto& derived() const noexcept
        {
            return vrmc::to_derived<derived_type>(*this);
        }

        template <typename TSelf>
        static decltype(auto) get_impl(TSelf&& self, entity_id eid) noexcept
        {
            auto i = self.entity_id_to_index(eid);
            ECST_ASSERT(self.valid_index(i));

            return vrmc::forward_like<TSelf>(self.derived()._data[i]);
        }

    public:
        auto valid_index(sz_t i) const noexcept
        {
            return i >= 0 && i < derived().capacity();
        }

        constexpr auto ECST_CONST_FN entity_id_to_index(entity_id eid) const
            noexcept
        {
            return vrmc::to_sz_t(eid);
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

        auto empty_free_ids() const noexcept
        {
            return derived()._free_ids.empty();
        }

        auto pop_free_id()
        {
            derived().grow_if_required();
            ECST_ASSERT(!empty_free_ids());

            auto result = derived()._free_ids.back();
            derived()._free_ids.pop_back();

            return result;
        }

        auto push_free_id(entity_id id) noexcept
        {
            return derived()._free_ids.add(id);
        }

        auto create_and_get_eid()
        {
            return pop_free_id();
        }

        auto alive(entity_id eid) const noexcept
        {
            // If `eid` is in use, then the entity is alive.
            return !derived()._free_ids.has(eid);
        }

        auto reclaim(entity_id eid)
        {
            return push_free_id(eid);
        }
    };
} // namespace ecst::ctx::storage::entity::container::impl
