// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./base.hpp"
#include <array>
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <vrm/core/experimental/sparse_set.hpp>

namespace ecst::context::storage::entity::container
{
#define ECST_IMPL_FIXED_BASE_TYPE \
    impl::base<fixed<TEntityMetadata, TCapacity>, TEntityMetadata>

    template <typename TEntityMetadata, sz_t TCapacity>
    class fixed : public ECST_IMPL_FIXED_BASE_TYPE
    {
        friend ECST_IMPL_FIXED_BASE_TYPE;
#undef ECST_IMPL_FIXED_BASE_TYPE

    private:
        using entity_metadata_type = TEntityMetadata;

        std::array<entity_metadata_type, TCapacity> _data;
        vrmc::fixed_vector_sparse_set<entity_id, TCapacity> _free_ids;

        constexpr auto capacity() const noexcept
        {
            return TCapacity;
        }

        void grow_if_required() const noexcept
        {
        }

    public:
        fixed()
        {
            add_range_in_set_reverse(
                _free_ids, entity_id{0}, entity_id{TCapacity});
        }
    };
} // namespace ecst::context::storage::entity::container
