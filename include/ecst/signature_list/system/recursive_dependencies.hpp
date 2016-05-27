// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include "./id.hpp"
#include "./dependents.hpp"
#include "./bf_traversal.hpp"

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    // TODO: refactor/beautify
    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemSignature>
        auto recursive_dependency_id_list_impl(
            TSystemSignatureList ssl, TSystemSignature ss)
        {
            // Initial dependencies of `ss` list.
            auto dependencies_list = // .
                signature_list::system::dependencies_ids_list(ssl, ss);

            // Recursive step implementation.
            auto step = [=](auto self, auto curr_list)
            {
                return bh::concat( // .
                    curr_list,         // .
                    bh::fold_right(curr_list, mp::list::empty_v,
                        [=](auto xid, auto acc)
                        {
                            auto xsig = signature_by_id(ssl, xid);
                            auto new_list = dependencies_ids_list(ssl, xsig);

                            return bh::concat(acc, self(new_list));
                        }));
            };

            // Start the recursion.
            return bh::unique(
                bh::sort(bh::fix(step)(dependencies_list)));
        }

        template <typename TSystemSignatureList,
            typename TStartSystemSignatureList>
        auto recursive_dependents_id_list_impl(
            TSystemSignatureList ssl, TStartSystemSignatureList parent_list)
        {
            auto parent_ids_list = bh::transform(parent_list, [ssl](auto x)
                {
                    return id_by_signature(ssl, x);
                });

            return bf_traversal::bf_traversal_impl(parent_ids_list, ssl);
        }
    }

    /// @brief Returns a list of the IDs of all dependencies (recursively) of
    /// `ss`.
    template <typename TSystemSignatureList, typename TSystemSignature>
    constexpr auto recursive_dependency_id_list(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        return decltype(impl::recursive_dependency_id_list_impl(ssl, ss)){};
    }

    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemSignature,
            typename TDependencySignature>
        auto has_dependency_recursive_impl(TSystemSignatureList ssl,
            TSystemSignature ss, TDependencySignature ds)
        {
            auto ds_id = id_by_signature(ssl, ds);

            return bh::contains(
                recursive_dependency_id_list(ssl, ss), ds_id);
        }
    }

    /// @brief Returns `true` if `ds` is in the recursive dependency list of
    /// `ss`.
    template <typename TSystemSignatureList, typename TSystemSignature,
        typename TDependencySignature>
    constexpr auto has_dependency_recursive(
        TSystemSignatureList ssl, TSystemSignature ss, TDependencySignature ds)
    {
        return decltype(impl::has_dependency_recursive_impl(ssl, ss, ds)){};
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
