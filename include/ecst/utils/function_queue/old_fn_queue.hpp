// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <ecst/config.hpp>
#include <ecst/debug.hpp>
#include "./dependencies.hpp"
#include "./utils.hpp"
#include "./aliases.hpp"
#include "./vtable.hpp"
#include "./fixed_storage.hpp"

ECST_NAMESPACE
{
    template <typename TSignature, sz_t TBufferSize>
    class fixed_function_queue;

    template <typename TReturn, typename... TArgs, sz_t TBufferSize>
    class fixed_function_queue<TReturn(TArgs...), TBufferSize>
    {
    private:
        using signature = TReturn(TArgs...);
        using vtable_type = complete_vtable_type<signature>;

        // TODO:
        using storage_type =
            impl::storage::fixed<signature, vtable_type, TBufferSize>;

        storage_type _storage;


    public:
        template <typename TF>
        void emplace(TF&& f)
        // TODO: noexcept
        {
            _storage.emplace(FWD(f));
        }

        auto call_all(TArgs... xs)
        // TODO: noexcept
        {
            _storage.call_all(xs...);
        }

        void clear()
        {
            _storage.clear();
        }
    };
}
ECST_NAMESPACE_END
