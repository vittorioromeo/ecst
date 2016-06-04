// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/core/type_aliases.hpp>
#include <vrm/core/experimental/sparse_set.hpp>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>
#include <ecst/settings.hpp>
#include <ecst/context/types.hpp>

ECST_NAMESPACE
{
    template <sz_t TCapacity>
    using fixed_set = vrmc::fixed_vector_sparse_set<entity_id, TCapacity>;

    using dynamic_set = vrmc::dynamic_vector_sparse_set<entity_id>;

    namespace impl
    {
        template <typename TSettings>
        auto dispatch_set_type() noexcept
        {
            return settings::dispatch_on_storage_type(TSettings{},
                [](auto fixed_capacity)
                {
                    return mp::type_c<fixed_set<fixed_capacity>>;
                },
                [](auto)
                {
                    return mp::type_c<dynamic_set>;
                });
        }

        template <typename TSettings>
        using dispatch_set =
            mp::unwrap<decltype(dispatch_set_type<TSettings>())>;
    }

    using impl::dispatch_set;

    namespace impl
    {
        // TODO: other usage opportunities? to vrm_core?
        template <typename T, typename TF>
        auto reverse_loop(T i_begin, T i_end, TF&& f)
        {
            for(T i = i_end; i-- > i_begin;)
            {
                f(i);
            }
        }
    }

    template <typename TSet, typename T>
    void add_range_in_set_reverse(TSet & set, T i_begin, T i_end)
    {
        impl::reverse_loop(i_begin, i_end, [&set](auto i)
            {
                set.unchecked_add(i);
            });
    }

    template <typename TSet, typename T>
    void add_range_in_set_forward(TSet & set, T i_begin, T i_end)
    {
        for(T i(i_begin); i < i_end; ++i)
        {
            set.unchecked_add(i);
        }
    }
}
ECST_NAMESPACE_END
