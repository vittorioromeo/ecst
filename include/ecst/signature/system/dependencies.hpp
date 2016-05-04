// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system/data.hpp>
#include <ecst/signature/system/is_signature.hpp>

ECST_SIGNATURE_SYSTEM_NAMESPACE
{
    template <typename TSystemSignature>
    constexpr auto dependencies_as_tag_list(TSystemSignature ss)
    {
        ECST_S_ASSERT_DT(is_signature(ss));
        return impl::signature_tag_depedency_list_type<TSystemSignature>{};
    }

    /// @brief Returns the number of dependencies of `ss`.
    template <typename TSystemSignature>
    constexpr auto dependency_count(TSystemSignature ss)
    {
        return mp::list::size(dependencies_as_tag_list(ss));
    }

    /// @brief Returns `true` if `ss` is independent.
    template <typename TSystemSignature>
    constexpr auto is_independent(TSystemSignature ss)
    {
        return mp::bool_v<(dependency_count(ss) == 0)>;
    }
}
ECST_SIGNATURE_SYSTEM_NAMESPACE_END