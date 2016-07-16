// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./multi.hpp"

#define ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE                     \
    template <typename TSystemSignature, typename TEDFunctions, \
        typename TContext>

#define ECST_IMPL_MULTI_DATA_PROXY \
    multi<TSystemSignature, TEDFunctions, TContext>

ECST_CONTEXT_SYSTEM_NAMESPACE
{
    namespace data_proxy
    {
        ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE
        ECST_IMPL_MULTI_DATA_PROXY::multi(                              // .
            TEDFunctions&& functions, TContext& context, sz_t ep_count, // .
            sz_t ae_count,                                              // .
            sz_t oe_count                                               // .
            ) noexcept
            : base_type{std::move(functions), context, ep_count}, // .
              _ae_count{ae_count},                                // .
              _oe_count{oe_count}                                 // .
        {
        }

        ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_MULTI_DATA_PROXY::for_other_entities(TF&& f)
        {
            return this->_functions._f_for_other_entities(f);
        }

        ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE
        template <typename TF>
        auto ECST_IMPL_MULTI_DATA_PROXY::for_all_entities(TF&& f)
        {
            return this->_functions._f_for_all_entities(f);
        }

        ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE
        auto ECST_IMPL_MULTI_DATA_PROXY::all_entity_count() const noexcept
        {
            return _ae_count;
        }

        ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE
        auto ECST_IMPL_MULTI_DATA_PROXY::other_entity_count() const noexcept
        {
            return _oe_count;
        }
    }
}
ECST_CONTEXT_SYSTEM_NAMESPACE_END

#undef ECST_IMPL_MULTI_DATA_PROXY
#undef ECST_IMPL_MULTI_DATA_PROXY_TEMPLATE
