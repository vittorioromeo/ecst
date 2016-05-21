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
            return static_if(settings::has_fixed_entity_storage<TSettings>)
                .then([](auto ts)
                    {
                        return mp::type_c<
                            fixed_set<settings::fixed_capacity(ts)>>;
                    })
                .else_([](auto)
                    {
                        return mp::type_c<dynamic_set>;
                    })(TSettings{});
        }

        template <typename TSettings>
        using dispatch_set =
            mp::unwrap<decltype(dispatch_set_type<TSettings>())>;
    }

    using impl::dispatch_set;

    template <typename TSet, typename T>
    void add_range_in_set_reverse(TSet & set, T i_begin, T i_end)
    {
        // TODO: could be unchecked

        for(T i(i_end - 1); i > i_begin; --i)
        {
            set.add(i);
        }

        set.add(i_begin);
    }

    template <typename TSet, typename T>
    void add_range_in_set_forward(TSet & set, T i_begin, T i_end)
    {
        // TODO: could be unchecked

        for(T i(i_begin); i < i_end; ++i)
        {
            set.add(i);
        }
    }
}
ECST_NAMESPACE_END
