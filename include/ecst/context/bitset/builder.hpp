// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>
#include <ecst/settings.hpp>
#include <ecst/tag.hpp>
#include "./bits.hpp"
#include "./data.hpp"

ECST_CONTEXT_BITSET_NAMESPACE
{
    /// @brief Sets all the bits corresponding to the component types in `scl`
    /// to `1` in `bitset`.
    template <typename TSettings, typename TSetComponentsList>
    void fill(dispatch<TSettings> & bitset, TSettings s,
        TSetComponentsList scl) noexcept
    {
        ECST_S_ASSERT_DT(tag::component::is_valid(scl));
        auto csl = settings::component_signature_list(s);

        mp::bh::for_each(scl, [&](auto ct)
            {
                auto id(signature_list::component::id_by_tag(csl, ct));
                bitset.set_component_by_id(id, true);
            });
    }

    /// @brief Returns a bitset filled with `scl`'s component bits.
    template <typename TSettings, typename TSetComponentsList>
    auto make(TSettings s, TSetComponentsList scl) noexcept
    {
        dispatch<TSettings> result;
        fill(result, s, scl);
        return result;
    }

    /// @brief Returns a bitset matching `TSystemSignature`'s used components.
    template <typename TSystemSignature, typename TSettings>
    auto make_from_system_signature(TSettings s) noexcept
    {
        auto read_component_tag_list =
            signature::system::read_component_tag_list_type<TSystemSignature>{};

        auto write_component_tag_list =
            signature::system::write_component_tag_list_type<
                TSystemSignature>{};

        auto all_component_tag_list =
            mp::bh::concat(read_component_tag_list, write_component_tag_list);

        return make(s, all_component_tag_list);
    }
}
ECST_CONTEXT_BITSET_NAMESPACE_END
