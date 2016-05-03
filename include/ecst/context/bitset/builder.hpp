// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/context/types.hpp>
#include <ecst/settings.hpp>
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
        ECST_S_ASSERT_DT(signature::component::is_tag_list(scl));
        auto csl = settings::component_signature_list(s);

        for_tuple(
            [&](auto ct)
            {
                auto id(signature_list::component::id_by_tag(csl, ct));
                bitset.set_component_by_id(id, true);
            },
            scl);
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
        auto component_uses =
            signature::system::component_uses_type<TSystemSignature>{};

        auto set_component_tags = mp::list::transform(
            [](auto x)
            {
                using component_type = typename decltype(x)::component;
                return signature::component::tag<component_type>;
            },
            component_uses);

        return make(s, set_component_tags);
    }
}
ECST_CONTEXT_BITSET_NAMESPACE_END
