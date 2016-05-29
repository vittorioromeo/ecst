// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>

ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE
{
    namespace chunk
    {
        /// @brief Chunk metadata type.
        /// @details Stored in a tuple, inside entity metadata.
        template <typename TChunk>
        using metadata = typename TChunk::metadata;

        /// @brief Type of the component stored by `TChunk`.
        template <typename TChunk>
        using component = typename TChunk::component_type;

        // TODO: move
        namespace impl
        {
            template <typename TComponentTagList>
            auto make_component_tuple_type_impl(TComponentTagList ctl)
            {
                return bh::transform(ctl, [](auto ct)
                    {
                        return mp::type_c<                                  // .
                            tag::component::unwrap<ECST_DECAY_DECLTYPE(ct)> // .
                            >;
                    });
            }

            template <typename TComponentTagList>
            constexpr auto make_component_tuple_type(TComponentTagList ctl)
            {
                return decltype(make_component_tuple_type_impl(ctl)){};
            }

            // TODO: use bh tuple
            template <typename TComponentTagList>
            using component_tuple_type =
                mp::list::unwrap_tuple<ECST_DECAY_DECLTYPE(
                    make_component_tuple_type(TComponentTagList{}))>;
        }
    }
}
ECST_CONTEXT_STORAGE_COMPONENT_NAMESPACE_END
