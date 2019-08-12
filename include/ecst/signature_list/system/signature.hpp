// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/mp/list.hpp>
#include <ecst/signature/system/tag_of.hpp>
#include <ecst/signature_list/system/is_signature_list.hpp>
#include <ecst/tag/system.hpp>

ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE
{
    namespace impl
    {
        template <typename TSystemSignatureList, typename TSystemTag>
        constexpr auto signature_by_tag_impl(
            TSystemSignatureList ssl, TSystemTag st)
        {
            ECST_S_ASSERT_DT(valid(ssl));
            ECST_S_ASSERT_DT(tag::system::valid(st));

            return *bh::find_if(ssl, [st](auto x_ssig) {
                return mp::same_type_decay(
                    signature::system::tag_of(x_ssig), st);
            });
        }
    } // namespace impl

    /// @brief Given a system tag `st`, returns its signature.
    template <typename TSystemSignatureList, typename TSystemTag>
    constexpr auto signature_by_tag(TSystemSignatureList ssl, TSystemTag st)
    {
        return decltype(impl::signature_by_tag_impl(ssl, st)){};
    }

    /// @brief Given a system type `TSystem`, returns its signature.
    template <typename TSystem, typename TSystemSignatureList>
    constexpr auto signature_by_type(TSystemSignatureList ssl)
    {
        return signature_by_tag(ssl, tag::system::v<TSystem>);
    }

    /// @brief Given a system ID `id`, returns its signature.
    template <typename TSystemSignatureList, typename TID>
    constexpr auto signature_by_id(TSystemSignatureList ssl, TID id)
    {
        return bh::at(ssl, id);
    }
}
ECST_SIGNATURE_LIST_SYSTEM_NAMESPACE_END
