// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include "./data.hpp"
#include "./valid.hpp"

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    template <typename TSystemSignature>
    constexpr auto dependencies_as_tag_list(TSystemSignature ss)
    {
        ECST_S_ASSERT_DT(valid(mp::unwrapped(ss)));
        return tag_depedency_list_type<TSystemSignature>{};
    }

    /// @brief Returns the number of dependencies of `ss`.
    template <typename TSystemSignature>
    constexpr auto dependency_count(TSystemSignature ss)
    {
        return bh::size(dependencies_as_tag_list(ss));
    }

    namespace impl
    {
        struct independent_t
        {
            template <typename TSystemSignature>
            constexpr auto operator()(TSystemSignature ss) const noexcept
            {
                return dependency_count(ss) == mp::sz_v<0>;
            }
        };
    }

    /// @brief Returns `true` if `ss` is independent.
    constexpr impl::independent_t indepedent{};
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END
