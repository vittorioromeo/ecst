// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <type_traits>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/settings.hpp>
#include "./chunk.hpp"

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace impl
    {
        template <typename TSettings>
        class data
        {
        public:
            using chunk_tuple = chunk_tuple_type<TSettings>;
            using chunk_metadata_tuple = chunk_metadata_tuple_type<TSettings>;

        private:
            chunk_tuple _chunk_tuple;

            /// @brief Returns a chunk by chunk ID.
            template <typename TID>
            auto& chunk_by_id(TID) noexcept
            {
                return bh::at(_chunk_tuple, TID{});
            }

            /// @brief Returns the index of the chunk storing `ct`.
            template <typename TComponentTag>
            constexpr auto chunk_idx(TComponentTag ct) noexcept
            {
                return mp::list::index_of_first_matching(_chunk_tuple,
                    [ct](auto& c)
                    {
                        using chunk_type = ECST_DECAY_DECLTYPE(c);

                        using chunk_component_tag_list_type = // .
                            typename chunk_type::component_tag_list_type;

                        return bh::contains(
                            chunk_component_tag_list_type{}, ct);
                    });
            }

            /// @brief Returns the chunk storing `TComponent`.
            template <typename TComponentTag>
            auto& chunk_for(TComponentTag ct) noexcept
            {
                return bh::at(_chunk_tuple, chunk_idx(ct));
            }

            /// @brief Executes `f` on the chunk storing `TComponent`.
            template <typename TComponentTag, typename TSelf,
                typename TEntityChunkMetadata, typename TF>
            decltype(auto) chunk_fn_impl(TComponentTag ct, TSelf&& self,
                TEntityChunkMetadata& ecm, TF&& f)
            {
                decltype(auto) c(self.chunk_for(ct));
                auto& metadata = bh::at(ecm, chunk_idx(ct));
                return f(c, metadata);
            }

            template <typename TComponentTag, typename TSelf,
                typename TEntityChunkMetadata>
            decltype(auto) get_impl(TComponentTag ct, TSelf&& self,
                entity_id eid, const TEntityChunkMetadata& ecm)
            {
                return chunk_fn_impl(ct, FWD(self), ecm,
                    [ct, eid](auto& chunk, const auto& md) -> decltype(auto)
                    {
                        return chunk.get(ct, eid, md);
                    });
            }

            template <typename TComponentTag, typename TSelf,
                typename TEntityChunkMetadata>
            decltype(auto) add_impl(TComponentTag ct, TSelf&& self,
                entity_id eid, TEntityChunkMetadata& ecm)
            {
                ELOG(                                                    // .
                    debug::lo_component() << "Creating for eID: " << eid // .
                                          << "\n";                       // .
                    );

                return chunk_fn_impl(ct, FWD(self), ecm,
                    [ct, eid](auto& chunk, auto& md) -> decltype(auto)
                    {
                        return chunk.add(ct, eid, md);
                    });
            }

        public:
            /// @brief Given an "entity id" and an "entity chunk metadata",
            /// return a reference to a component.
            template <typename TComponentTag, typename... Ts>
                auto& get(TComponentTag ct, Ts&&... xs) & noexcept
            {
                return get_impl(ct, *this, FWD(xs)...);
            }

            /// @brief Given an "entity id" and an "entity chunk metadata",
            /// return a const reference to a component.
            template <typename TComponentTag, typename... Ts>
            const auto& get(TComponentTag ct, Ts&&... xs) const& noexcept
            {
                return get_impl(ct, *this, FWD(xs)...);
            }

            /// @brief Given an "entity id" and an "entity chunk metadata",
            /// creates a component and returns a reference to it.
            template <typename TComponentTag, typename... Ts>
                decltype(auto) add(TComponentTag ct, Ts&&... xs) & noexcept
            {
                return add_impl(ct, *this, FWD(xs)...);
            }
        };
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
