// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature_list/system/id.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename TSystemSignatureList>
        auto id_list_impl(TSystemSignatureList ssl)
        {
            return mp::list::transform(
                [=](auto x_sig)
                {
                    return id_by_signature(ssl, x_sig);
                },
                ssl);
        }
    }

    /// @brief Returns the list of system IDs inside a system signature list.
    template <typename TSystemSignatureList>
    constexpr auto id_list(TSystemSignatureList ssl)
    {
        return decltype(impl::id_list_impl(ssl)){};
    }

    /// @brief Executes `f` on every system ID inside `ssl`.
    template <typename TF, typename TSystemSignatureList>
    constexpr auto for_ids(TF && f, TSystemSignatureList ssl)
    {
        return for_tuple(f, id_list(ssl));
    }

    namespace impl
    {
        template <typename TSystemSignature, typename TSystemSignatureList>
        constexpr auto dependencies_as_id_list_impl(
            TSystemSignatureList ssl, TSystemSignature ss)
        {
            auto tag_list = signature::system::dependencies_as_tag_list(ss);
            return mp::list::transform(
                [=](auto x_tag)
                {
                    return id_by_tag(ssl, x_tag);
                },
                tag_list);
        }
    }

    /// @brief Given a signature list and a signature, returns the list of the
    /// signature's dependencies, as IDs.
    template <typename TSystemSignature, typename TSystemSignatureList>
    constexpr auto dependencies_as_id_list(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        return decltype(impl::dependencies_as_id_list_impl(ssl, ss)){};
    }

    namespace impl
    {
        template <typename TSystemSignature, typename TSystemSignatureList>
        constexpr auto dependencies_of_impl(
            TSystemSignatureList ssl, TSystemSignature ss)
        {
            auto tag_list = signature::system::dependencies_as_tag_list(ss);
            return mp::list::transform(
                [=](auto x_tag)
                {
                    return signature_by_tag(ssl, x_tag);
                },
                tag_list);
        }
    }

    /// @brief Given a signature list and a signature, returns the list of the
    /// signature's dependencies, as IDs.
    template <typename TSystemSignature, typename TSystemSignatureList>
    constexpr auto dependencies_of(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        return decltype(impl::dependencies_of_impl(ssl, ss)){};
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
