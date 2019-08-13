// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <ecst/context/bitset.hpp>
#include <ecst/context/storage/component.hpp>
#include <ecst/context/types.hpp>

namespace ecst::context::entity
{
    namespace impl
    {
        template <typename TBitset, typename TChunkMetadata>
        class metadata
        {
            template <typename>
            friend class ::ecst::context::storage::entity::impl::data;

        private:
            /// @brief Bitset keeping track of the components of the entity.
            TBitset _bitset;

            /// @brief Validity counter used for handle management.
            counter _counter;

            // TODO:
            TChunkMetadata _chunkMetadata;

            /// @brief Returns the tuple of metadata required to get the
            /// component data from the component storage chunks.
            auto& as_chunk_metadata() noexcept;

            /// @brief Returns the tuple of metadata required to get the
            /// component data from the component storage chunks. (const
            /// version)
            const auto& as_chunk_metadata() const noexcept;

        public:
            /// @brief Clears the bitset and increments the counter.
            void reset() noexcept;

            template <typename TComponent>
            auto remove_component() noexcept;

            /// @brief Returns `true` if the entity has the `TComponent` bit.
            template <typename TComponent>
            auto has_component() const noexcept;

            /// @brief Given a component storage and the entity id (which is the
            /// index of the entity metadata in the entity storage), returns a
            /// reference to the component data.
            template <typename TComponent, typename TComponentStorage>
            auto& get_component(TComponentStorage& cs, entity_id eid) noexcept;

            /// @brief Given a component storage and the entity id (which is the
            /// index of the entity metadata in the entity storage), returns a
            /// reference to the component data. (const version)
            template <typename TComponent, typename TComponentStorage>
            const auto& get_component(
                TComponentStorage& cs, entity_id eid) const noexcept;

            /// @brief Given a component storage and the entity id, creates (and
            /// potentially allocates memory for) a component instance,
            /// returning a reference to it.
            template <typename TComponent, typename TComponentStorage>
            decltype(auto) add_component(
                TComponentStorage& cs, entity_id eid) noexcept;

            /// @brief Returns the component bitset.
            const auto& bitset() const noexcept;

            // TODO:
            constexpr operator TChunkMetadata() const
            {
                return {};
            }
        };
    } // namespace impl
} // namespace ecst::context::entity
