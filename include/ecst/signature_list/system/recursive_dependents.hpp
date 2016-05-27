// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include "./dependents.hpp"
#include "./bf_traversal.hpp"

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename TSystemSignatureList,
            typename TStartSystemSignatureList>
        auto recursive_dependents_id_list_impl(
            TSystemSignatureList ssl, TStartSystemSignatureList parent_list)
        {
            return bf_traversal::execute(id_list(ssl, parent_list), ssl);
        }
    }

    /// @brief Returns the set of dependent IDs of `parent`.
    template <typename TSystemSignatureList, typename TStartSystemSignatureList>
    constexpr auto recursive_dependents_id_list(
        TSystemSignatureList ssl, TStartSystemSignatureList parent_list)
    {
        return decltype(
            impl::recursive_dependents_id_list_impl(ssl, parent_list)){};
    }

    /// @brief Returns the number of unique systems that recursively depend on
    /// `st`.
    template <typename TSystemSignatureList, typename TStartSystemTagList>
    constexpr auto chain_size(
        TSystemSignatureList ssl, TStartSystemTagList sstl)
    {
        ECST_S_ASSERT(tag::system::is_list(sstl));
        return bh::size(recursive_dependents_id_list(
            ssl, signature_list_from_tag_list(ssl, sstl)));
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
