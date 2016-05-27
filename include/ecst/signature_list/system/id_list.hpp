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
            return bh::transform(ssl, [=](auto x_sig)
                {
                    return id_by_signature(ssl, x_sig);
                });
        }

        // TODO: move
        template <typename TSystemSignatureList, typename TSystemTagList>
        auto signature_list_from_tag_list_impl(
            TSystemSignatureList ssl, TSystemTagList stl) noexcept
        {
            return bh::transform(stl, [ssl](auto st)
                {
                    return signature_by_tag(ssl, st);
                });
        }
    }

    // TODO: move, docs
    template <typename TSystemSignatureList, typename TSystemTagList>
    constexpr auto signature_list_from_tag_list(
        TSystemSignatureList ssl, TSystemTagList stl) noexcept
    {
        return decltype(impl::signature_list_from_tag_list_impl(ssl, stl)){};
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
        return bh::for_each(id_list(ssl), f);
    }

    namespace impl
    {
        template <typename TSystemSignature, typename TSystemSignatureList>
        constexpr auto dependencies_as_id_list_impl(
            TSystemSignatureList ssl, TSystemSignature ss)
        {
            auto tag_list = signature::system::dependencies_as_tag_list(ss);
            return bh::transform(tag_list, [=](auto x_tag)
                {
                    return id_by_tag(ssl, x_tag);
                });
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

    /// @brief Given a signature list and a signature, returns the list of the
    /// signature's dependencies, as IDs.
    template <typename TSystemSignature, typename TSystemSignatureList>
    constexpr auto dependencies_of(
        TSystemSignatureList ssl, TSystemSignature ss)
    {
        constexpr auto tl = signature::system::dependencies_as_tag_list(ss);
        return signature_list_from_tag_list(ssl, tl);
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
