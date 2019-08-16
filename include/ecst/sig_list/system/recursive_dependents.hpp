// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./bf_traversal.hpp"
#include "./dependents.hpp"
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/sig_list/system/sig_list_from_tag_list.hpp>

namespace ecst::sig_list::system
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
    } // namespace impl

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
        static_assert(tag::system::is_list(sstl));
        return bh::size(recursive_dependents_id_list(
            ssl, sig_list_from_tag_list(ssl, sstl)));
    }
} // namespace ecst::sig_list::system
