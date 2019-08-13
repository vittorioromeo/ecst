// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./id.hpp"
#include "./id_list.hpp"
#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>

namespace ecst::signature_list::system
{
    /// @brief Given a list of system tags, returns a list of system signatures.
    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto tag_list_to_signature_list(
        TSystemSignatureList ssl, TSystemTagList stl)
    {
        ECST_S_ASSERT_DT(tag::system::is_list(stl));

        return bh::transform(stl, [ssl](auto x) {
            return signature_list::system::signature_by_tag(ssl, x);
        });
    }

    /// @brief Given a list of system tags, returns a list of system IDs.
    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto tag_list_to_id_list(
        TSystemSignatureList ssl, TSystemTagList stl)
    {
        ECST_S_ASSERT_DT(tag::system::is_list(stl));

        return bh::transform(stl, [ssl](auto x) {
            return signature_list::system::id_by_tag(ssl, x);
        });
    }

    /// @brief Returns the set of IDs `parent` depends on.
    template <typename TSystemSignatureList, typename TSystemSignature>
    constexpr auto dependencies_ids_list(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        return tag_list_to_id_list(
            ssl, signature::system::dependencies_as_tag_list(ss));
    }

    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemSignature>
        auto dependent_ids_list_impl(
            TSystemSignatureList ssl, TSystemSignature parent)
        {
            namespace ss = signature::system;
            namespace sls = signature_list::system;

            // Retrieve the id of `parent`.
            auto parent_id = sls::id_by_signature(ssl, parent);

            // Build a list of dependent IDs.
            return bh::fold_right(
                ssl, mp::list::empty_v, [=](auto ss, auto acc) {
                    // Check if the ID of `parent` is in the list of `ss`'s
                    // depedendencies.
                    auto dl = sls::dependencies_as_id_list(ssl, ss);
                    if constexpr(bh::contains(dl, parent_id))
                    {
                        // If so, add `ss`'s ID to the result list.
                        auto ss_id = sls::id_by_signature(ssl, ss);
                        return bh::append(acc, ss_id);
                    }
                    else
                    {
                        return acc;
                    }
                });
        }
    } // namespace impl

    /// @brief Returns the set of dependent IDs of `parent`.
    template <typename TSystemSignatureList, typename TSystemSignature>
    constexpr auto dependent_ids_list(
        TSystemSignatureList ssl, TSystemSignature parent)
    {
        return decltype(impl::dependent_ids_list_impl(ssl, parent)){};
    }
} // namespace ecst::signature_list::system
