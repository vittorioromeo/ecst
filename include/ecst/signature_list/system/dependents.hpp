// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/system/id.hpp>
#include <ecst/signature_list/system/id_list.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    /// @brief Given a list of system tags, returns a list of system signatures.
    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto tag_list_to_signature_list(
        TSystemSignatureList ssl, TSystemTagList stl)
    {
        return bh::transform(stl, [ssl](auto x)
            {
                return signature_list::system::signature_by_tag(ssl, x);
            });
    }

    /// @brief Given a list of system tags, returns a list of system IDs.
    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto tag_list_to_id_list(
        TSystemSignatureList ssl, TSystemTagList stl)
    {
        return bh::transform(stl, [ssl](auto x)
            {
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

    /// @brief Returns the set of dependent IDs of `parent`.
    template <typename TSystemSignatureList, typename TSystemSignature>
    auto dependent_ids_list(TSystemSignatureList ssl, TSystemSignature parent)
    {
        namespace ss = signature::system;
        namespace sls = signature_list::system;

        // Retrieve the id of `parent`.
        auto parent_id = sls::id_by_signature(ssl, parent);

        // Build a list of dependent IDs.
        return bh::fold_left(ssl, mp::list::empty_v, [=](auto acc, auto ss)
            {
                // Check if the ID of `parent` is in the list of `ss`'s
                // depedendencies.
                auto dl = sls::dependencies_as_id_list(ssl, ss);
                return static_if(mp::list::contains(dl, parent_id))
                    .then([=](auto x_acc)
                        {
                            // If so, add `ss`'s ID to the result list.
                            auto ss_id = sls::id_by_signature(ssl, ss);
                            return mp::list::append(x_acc, ss_id);
                        })
                    .else_([](auto x_acc)
                        {
                            return x_acc;
                        })(acc);
            });
    }

    /// @brief Returns the set of independent system signatures.
    template <typename TSystemSignatureList>
    constexpr auto independent_signature_list(TSystemSignatureList ssl)
    {
        return bh::filter(ssl, [](auto sx)
            {
                return signature::system::is_independent(sx);
            });
    }

    /// @brief Returns the set of independent system IDs.
    template <typename TSystemSignatureList>
    constexpr auto independent_ids_list(TSystemSignatureList ssl)
    {
        return bh::transform(independent_signature_list(ssl), [ssl](auto x)
            {
                return signature_list::system::id_by_signature(ssl, x);
            });
    }

    /// @brief Executes `f` on every independent system ID in `ssl`.
    template <typename TSystemSignatureList, typename TF>
    void for_indepedent_ids(TSystemSignatureList ssl, TF && f)
    {
        bh::for_each(independent_ids_list(ssl), FWD(f));
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
