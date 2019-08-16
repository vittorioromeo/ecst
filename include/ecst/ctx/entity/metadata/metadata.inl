// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./metadata.hpp"

namespace ecst::ctx::entity::impl
{
    template <typename TBitset, typename TChunkMetadata>
    auto& ECST_CONST_FN
    metadata<TBitset, TChunkMetadata>::as_chunk_metadata() noexcept
    {
        return _chunkMetadata;
    }

    template <typename TBitset, typename TChunkMetadata>
    const auto& ECST_CONST_FN
    metadata<TBitset, TChunkMetadata>::as_chunk_metadata() const noexcept
    {
        return _chunkMetadata;
    }

    template <typename TBitset, typename TChunkMetadata>
    void metadata<TBitset, TChunkMetadata>::reset() noexcept
    {
        _bitset.clear();
        ++_counter;
    }

    template <typename TBitset, typename TChunkMetadata>
    template <typename TComponent>
    auto metadata<TBitset, TChunkMetadata>::remove_component() noexcept
    {
        return _bitset.template set_component<TComponent>(false);
    }

    template <typename TBitset, typename TChunkMetadata>
    template <typename TComponent>
    auto metadata<TBitset, TChunkMetadata>::has_component() const noexcept
    {
        return _bitset.template has_component<TComponent>();
    }

    template <typename TBitset, typename TChunkMetadata>
    template <typename TComponent, typename TComponentStorage>
    auto& ECST_PURE_FN metadata<TBitset, TChunkMetadata>::get_component(
        TComponentStorage& cs, entity_id eid) noexcept
    {
        ECST_ASSERT(has_component<TComponent>());

        constexpr auto ct = tag::component::v<TComponent>;
        return cs.get(ct, eid, as_chunk_metadata());
    }

    template <typename TBitset, typename TChunkMetadata>
    template <typename TComponent, typename TComponentStorage>
    const auto& ECST_PURE_FN metadata<TBitset, TChunkMetadata>::get_component(
        TComponentStorage& cs, entity_id eid) const noexcept
    {
        ECST_ASSERT(has_component<TComponent>());

        constexpr auto ct = tag::component::v<TComponent>;
        return cs.get(ct, eid, as_chunk_metadata());
    }

    template <typename TBitset, typename TChunkMetadata>
    template <typename TComponent, typename TComponentStorage>
    decltype(auto) metadata<TBitset, TChunkMetadata>::add_component(
        TComponentStorage& cs, entity_id eid) noexcept
    {
        _bitset.template set_component<TComponent>(true);
        ECST_ASSERT(has_component<TComponent>());

        constexpr auto ct = tag::component::v<TComponent>;
        return cs.add(ct, eid, as_chunk_metadata());
    }

    template <typename TBitset, typename TChunkMetadata>
    const auto& ECST_PURE_FN metadata<TBitset, TChunkMetadata>::bitset() const
        noexcept
    {
        return _bitset;
    }
} // namespace ecst::ctx::entity::impl
