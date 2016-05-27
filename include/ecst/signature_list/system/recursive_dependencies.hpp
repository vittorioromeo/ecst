// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include "./id.hpp"

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
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
                return bh::concat( //
                    curr_list,     // .
                    bh::fold_right(curr_list, mp::list::empty_v,
                        [=](auto xid, auto acc)
                        {
                            auto xsig = signature_by_id(ssl, xid);
                            auto new_list = dependencies_ids_list(ssl, xsig);

                            return bh::concat(acc, self(new_list));
                        }));
            };

            // Start the recursion.
            return bh::unique(bh::sort(bh::fix(step)(dependencies_list)));
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

            return bh::contains(recursive_dependency_id_list(ssl, ss), ds_id);
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

}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
