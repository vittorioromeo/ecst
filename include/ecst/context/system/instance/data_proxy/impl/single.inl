// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./single.hpp"

#define ECST_IMPL_SINGLE_DATA_PROXY_TEMPLATE                    \
    template <typename TSystemSignature, typename TEDFunctions, \
        typename TContext>

#define ECST_IMPL_SINGLE_DATA_PROXY \
    single<TSystemSignature, TEDFunctions, TContext>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        ECST_IMPL_SINGLE_DATA_PROXY_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_SINGLE_DATA_PROXY::for_other_entities(TF&&) noexcept
        {
            return ecst::impl::do_nothing;
        }

        ECST_IMPL_SINGLE_DATA_PROXY_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_SINGLE_DATA_PROXY::for_all_entities(TF&& f)
        {
            return this->for_entities(FWD(f));
        }

        ECST_IMPL_SINGLE_DATA_PROXY_TEMPLATE
        auto ECST_IMPL_SINGLE_DATA_PROXY::all_entity_count() const noexcept
        {
            return this->entity_count();
        }

        ECST_IMPL_SINGLE_DATA_PROXY_TEMPLATE
        constexpr auto ECST_IMPL_SINGLE_DATA_PROXY::other_entity_count() const
            noexcept
        {
            return sz_t(0);
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_SINGLE_DATA_PROXY
#undef ECST_IMPL_SINGLE_DATA_PROXY_TEMPLATE
